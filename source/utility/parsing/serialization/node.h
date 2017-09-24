#ifndef HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
#define HEADER_NODE_94200784_8C23_4200_B54C_65736B455736


#include <string>

#include "utility/parsing/node.h"
#include "utility/parsing/implementation/node.h"
#include "utility/convertion/implementation/convert.h"

#include <iostream>

namespace utility {
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
protected:
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
    INode::TSharedPtr       getNodeDefaults() const;

    INode::TConstSharedPtr  m_node_source;
    INode::TSharedPtr       m_node_destination;
    std::string             m_path;

protected:
    /// root node
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

protected:
    ///
    NodeList(Node<void> const &parent, std::string const &name);

private:
    ///
    Node<void> const &m_parent;
    ///
    std::list<TNode> m_nodes;
};


// ----- implementation


// ----- Node


template<typename TValue>
Node<TValue>::Node(Node<void> const &parent, std::string const &name, TValue const &default_value) {
    m_path = parent.m_path + "/" + name;
    if (parent.m_node_source)
        m_node_source       = parent.m_node_source->getChild(name);

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
    if (parent.m_node_source)
        for (auto const &i: *parent.m_node_source)
            if (i->getName() == name)
                m_nodes.push_back(TNode(i, parent.m_node_destination->addChild(name), parent.m_path));
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
    m_parent.m_node_destination->addChild(node.getNodeDefaults());
    m_nodes.push_back(node);
}


// -----


std::string convertCamelToDashed(std::string const &camel);


} // serialization
} // parsing
} // utility


#define DEFINE_ROOT_NODE_BEGIN(class_name) \
    struct T##class_name : protected utility::parsing::serialization::Node<void> { \
        T##class_name(utility::parsing::INode::TConstSharedPtr const &node): utility::parsing::serialization::Node<void> \
        (node, utility::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        T##class_name(): utility::parsing::serialization::Node<void> \
        (utility::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        utility::parsing::INode::TSharedPtr getNodeDefaults() const { \
            return utility::parsing::serialization::Node<void>::getNodeDefaults(); \
        }


#define DEFINE_ROOT_NODE_BEGIN_TYPED(class_name) \
    DEFINE_ROOT_NODE_BEGIN(class_name)


#define DEFINE_ROOT_NODE_END() \
    };


#define DEFINE_NODE_BEGIN(class_name) \
    struct T##class_name : protected utility::parsing::serialization::Node<void> { \
        T##class_name(utility::parsing::serialization::Node<void> const &parent): utility::parsing::serialization::Node<void> \
        (parent, utility::parsing::serialization::convertCamelToDashed(#class_name)) {}


#define DEFINE_NODE_END(class_name) \
    } class_name = *this;


#define DEFINE_ATTRIBUTE_DEFAULT(type, class_name, default_value) \
    struct T##class_name : public utility::parsing::serialization::Node<type> { \
        T##class_name(utility::parsing::serialization::Node<void> const &parent): utility::parsing::serialization::Node<type> \
        (parent, utility::parsing::serialization::convertCamelToDashed(#class_name), default_value) {} \
        using utility::parsing::serialization::Node<type>::operator =; \
    } class_name = *this;


#define DEFINE_ATTRIBUTE(type, class_name) \
    struct T##class_name : public utility::parsing::serialization::Node<type> { \
        T##class_name(utility::parsing::serialization::Node<void> const &parent) : utility::parsing::serialization::Node<type> \
        (parent, utility::parsing::serialization::convertCamelToDashed(#class_name)) {} \
        using utility::parsing::serialization::Node<type>::operator =; \
    } class_name = *this;


// todo: push_back
#define DEFINE_NODE_LIST_BEGIN(class_name) \
    struct T##class_name : public utility::parsing::serialization::Node<void> { \
        T##class_name( \
        utility::parsing::INode::TConstSharedPtr const &node_source, \
        utility::parsing::INode::TSharedPtr const &node_destination, \
        std::string const &path): \
        utility::parsing::serialization::Node<void> \
        (node_source, node_destination, utility::parsing::serialization::convertCamelToDashed(#class_name), path) {} \
        T##class_name(): utility::parsing::serialization::Node<void> \
        (utility::parsing::serialization::convertCamelToDashed(#class_name)) {} \


#define DEFINE_NODE_LIST_END(class_name) \
    }; \
    struct T##class_name##List : public utility::parsing::serialization::NodeList<T##class_name> { \
        T##class_name##List(utility::parsing::serialization::Node<void> const &parent) : utility::parsing::serialization::NodeList<T##class_name> \
        (parent, utility::parsing::serialization::convertCamelToDashed(#class_name)) {} \
    } class_name = *this;


#endif // HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
