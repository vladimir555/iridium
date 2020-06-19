// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
#define HEADER_NODE_94200784_8C23_4200_B54C_65736B455736


#include <string>

#include "iridium/parsing/node.h"
#include "iridium/parsing/implementation/node.h"
#include "iridium/convertion/implementation/convert.h"
#include "iridium/macros/va_args.h"


namespace iridium {
namespace parsing {
namespace serialization {


// ----- interface


template<typename TValue>
class Node {
public:
    ///
   ~Node() = default;
    ///
    TValue get() const;
    ///
    void set(TValue const &value);
    ///
    bool isDefault() const;
    ///
    operator TValue() const;
    ///
    Node &operator= (TValue const &value);
    ///
    bool operator== (TValue const &value) const;
    ///
    INode::TSharedPtr getNode() const;
protected:
    /// list attribute
    Node(
        INode::TConstSharedPtr  const &node_source,
        INode::TSharedPtr       const &node_destination,
        std::string             const &name,
        std::string             const &path);
    Node(std::string const &name);
    /// attribute
    Node(Node<void> const &parent, std::string const &name, TValue const &default_value);
    /// attribute
    Node(Node<void> const &parent, std::string const &name);

    INode::TConstSharedPtr  m_node_source;
    INode::TSharedPtr       m_node_destination;
    std::string             m_path;
    bool                    m_is_default = false;
};


template<>
class Node<void> {
public:
    /// root node for desiarization only
    Node(std::string const &name);
    /// root node for serialization and desiarization
    Node(INode::TConstSharedPtr const &node, std::string const &name);
    ///
   ~Node() = default;
    ///
    INode::TSharedPtr       getNode() const;

    INode::TConstSharedPtr  m_node_source;
    INode::TSharedPtr       m_node_destination;
    std::string             m_path;

protected:
    /// list node
    Node(
        INode::TConstSharedPtr  const &node_source,
        INode::TSharedPtr       const &node_destination,
        std::string             const &name,
        std::string             const &path);
    /// node
    Node(Node<void> const &parent, std::string const &name);
};


template<typename TNode>
class NodeList {
public:
    ///
   ~NodeList() = default;
    ///
    typedef typename std::list<TNode> TNodes;
    typedef typename std::list<TNode>::iterator iterator;
    typedef typename std::list<TNode>::const_iterator const_iterator;
    ///
    iterator begin();
    ///
    iterator end();
    ///
    const_iterator begin() const;
    ///
    const_iterator end() const;
    ///
    size_t size() const;
    ///
    void add(TNode const &node);
    ///
    template<typename TValue>
    void add(TValue const &node);

protected:
    ///
    NodeList(Node<void> const &parent, std::string const &name);

private:
    ///
    Node<void> const &m_parent;
    ///
    std::list<TNode> m_nodes;
};


template<typename TNode>
class NodePtr {
public:
    NodePtr(Node<void> const &parent);
   ~NodePtr() = default;
    std::shared_ptr<TNode> get() const;

private:
    typename std::shared_ptr<TNode> m_node;
};


// ----- implementation


// ----- Node


template<typename TValue>
Node<TValue>::Node(
    INode::TConstSharedPtr  const &node_source,
    INode::TSharedPtr       const &node_destination,
    std::string             const &name,
    std::string             const &path)
{
    m_path = path + "/" + name;

    if (node_source && node_source->getName() == name)
        m_node_source   = node_source;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->

    m_node_destination  = node_destination;
}


template<typename TValue>
Node<TValue>::Node(std::string const &name) {
    m_path              = "/" + name;
    m_node_destination  = implementation::CNode::create(name);
}


template<typename TValue>
INode::TSharedPtr Node<TValue>::getNode() const {
    return m_node_destination; // ----->
}


template<typename TValue>
Node<TValue>::Node(Node<void> const &parent, std::string const &name, TValue const &default_value) {
    m_path = parent.m_path + "/" + name;
    if (parent.m_node_source)
        m_node_source       = parent.m_node_source->getChild(name);
    else
        m_node_source       = nullptr; // pvs warn

    if (m_node_source) {
        m_node_destination  = parent.m_node_destination->addChild(name, m_node_source->getValue());
    } else {
        m_node_destination  = parent.m_node_destination->addChild(name, convertion::convert<std::string>(default_value));
        m_is_default        = true;
    }
}


template<typename TValue>
Node<TValue>::Node(Node<void> const &parent, std::string const &name) {
    m_path = parent.m_path + "/" + name;
    if (parent.m_node_source)
        m_node_source       = parent.m_node_source->getChild(name);
    else
        m_node_source       = nullptr; // pvs warn

    if (m_node_source)
        m_node_destination  = parent.m_node_destination->addChild(name, m_node_source->getValue());
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
}


template<typename TValue>
TValue Node<TValue>::get() const {
    if (m_node_destination)
        return convertion::convert<TValue>(m_node_destination->getValue());
    else
        throw std::runtime_error("value " + m_path + " not serialized"); // ----->
}


template<typename TValue>
void Node<TValue>::set(TValue const &value) {
    m_node_destination->setValue(convertion::convert<std::string>(value));
}


template<typename TValue>
bool Node<TValue>::isDefault() const {
    return m_is_default;
}


template<typename TValue>
Node<TValue>::operator TValue() const {
    return get(); // ----->
}


template<typename TValue>
Node<TValue> &Node<TValue>::operator= (TValue const &value) {
    set(value);
    return *this; // ----->
}


template<typename TValue>
bool Node<TValue>::operator== (TValue const &value) const {
    return get() == value; // ----->
}
// -----


// ----- NodeList


template<typename TNode>
NodeList<TNode>::NodeList(Node<void> const &parent, std::string const &name)
:
    m_parent(parent)
{
    if (parent.m_node_source) {
        for (auto const &i: *parent.m_node_source)
            if (i->getName() == name)
                m_nodes.push_back(TNode(i, parent.m_node_destination->addChild(i->getName(), i->getValue()), parent.m_path));
    }
}


template<typename TNode>
typename NodeList<TNode>::iterator NodeList<TNode>::begin() {
    return m_nodes.begin(); // ----->
}


template<typename TNode>
typename NodeList<TNode>::iterator NodeList<TNode>::end() {
    return m_nodes.end(); // ----->
}


template<typename TNode>
typename NodeList<TNode>::const_iterator NodeList<TNode>::begin() const {
    return m_nodes.begin(); // ----->
}


template<typename TNode>
typename NodeList<TNode>::const_iterator NodeList<TNode>::end() const {
    return m_nodes.end(); // ----->
}


template<typename TNode>
size_t NodeList<TNode>::size() const {
    return m_nodes.size();
}


template<typename TNode>
void NodeList<TNode>::add(TNode const &node) {
    m_parent.m_node_destination->addChild(node.getNode());
    m_nodes.push_back(node);
}


template<typename TNode>
template<typename TValue>
void NodeList<TNode>::add(TValue const &value) {
    TNode node;
    node.set(value);
    add(node);
}
// -----


// ----- NodeList


template<typename TNode>
NodePtr<TNode>::NodePtr(Node<void> const &parent) {
    try {
        m_node = std::make_shared<TNode>(parent);
    } catch (...) {}
}


template<typename TNode>
std::shared_ptr<TNode> NodePtr<TNode>::get() const {
    return m_node; // ----->
}
// -----


std::string convertCamelToDashed(std::string const &camel);


} // serialization
} // parsing
} // iridium


#define DEFINE_ROOT_NODE_BEGIN(class_name) \
    struct T##class_name : protected iridium::parsing::serialization::Node<void> { \
        T##class_name(iridium::parsing::INode::TConstSharedPtr const &node): \
        iridium::parsing::serialization::Node<void> \
        (node, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        T##class_name(): iridium::parsing::serialization::Node<void> \
        (iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        iridium::parsing::INode::TSharedPtr getNode() const { \
            return iridium::parsing::serialization::Node<void>::getNode(); \
        } \
        T##class_name(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::Node<void> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {}


#define DEFINE_ROOT_NODE_END() \
    };


#define DEFINE_NODE_BEGIN(class_name) \
    struct T##class_name : protected iridium::parsing::serialization::Node<void> { \
        T##class_name(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::Node<void> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {}


#define DEFINE_NODE_END(class_name) \
    } class_name = *this;


#define DEFINE_ATTRIBUTE_2(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::Node<type> { \
        T##class_name(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::Node<type> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        using iridium::parsing::serialization::Node<type>::operator =; \
    } class_name = *this;


#define DEFINE_ATTRIBUTE_3(type, class_name, default_value) \
    struct T##class_name : public iridium::parsing::serialization::Node<type> { \
        typedef type TType; \
        T##class_name(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::Node<type> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name), default_value) {} \
        using iridium::parsing::serialization::Node<type>::operator =; \
    } class_name = *this;


#define DEFINE_ATTRIBUTE(...) \
    dMACRO_CHOOSER(DEFINE_ATTRIBUTE, __VA_ARGS__)(__VA_ARGS__)


#define DEFINE_NODE_LIST_BEGIN(class_name) \
    struct T##class_name : public iridium::parsing::serialization::Node<void> { \
        T##class_name( \
        iridium::parsing::INode::TConstSharedPtr const &node_source, \
        iridium::parsing::INode::TSharedPtr const &node_destination, \
        std::string const &path): \
        iridium::parsing::serialization::Node<void> \
        (node_source, node_destination, iridium::parsing::serialization::convertCamelToDashed(#class_name), path) {} \
        T##class_name(): iridium::parsing::serialization::Node<void> \
        (iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \


#define DEFINE_NODE_LIST_END(class_name) \
    }; \
    struct T##class_name##List : public iridium::parsing::serialization::NodeList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::NodeList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
    } class_name = *this;


#define DEFINE_ATTRIBUTE_LIST(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::Node<type> { \
        T##class_name(iridium::parsing::serialization::Node<void> const &parent): \
            iridium::parsing::serialization::Node<type> \
                (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        using iridium::parsing::serialization::Node<type>::operator =; \
        T##class_name( \
            iridium::parsing::INode::TConstSharedPtr const &node_source, \
            iridium::parsing::INode::TSharedPtr const &node_destination, \
            std::string const &path): \
                iridium::parsing::serialization::Node<type> \
                    (node_source, node_destination, iridium::parsing::serialization::convertCamelToDashed(#class_name), path) {} \
        T##class_name(): \
            iridium::parsing::serialization::Node<type> \
                (iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
    }; \
    struct T##class_name##List : public iridium::parsing::serialization::NodeList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::Node<void> const &parent): \
        iridium::parsing::serialization::NodeList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToDashed(#class_name)) {} \
    } class_name = *this;


#define DEFINE_NODE_EXTERNAL(class_name) \
    T##class_name class_name = *this;


#define DEFINE_NODE_PTR(class_name) \
    NodePtr<T##class_name> class_name##_ptr = *this;


#endif // HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
