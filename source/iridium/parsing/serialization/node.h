// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
#define HEADER_NODE_94200784_8C23_4200_B54C_65736B455736


#include <string>
#include <stdexcept>

#include "iridium/parsing/node.h"
#include "iridium/parsing/implementation/node.h"
#include "iridium/macros/va_args.h"


namespace iridium {
namespace parsing {
namespace serialization {


// ----- interface


// todo: copy constructor; rm TType; fix List property name


template<typename TValue>
class NodeView {
public:
    ///
   ~NodeView() = default;
    /// todo: lazy convertion
    TValue get() const;
    ///
    void set(TValue const &value);
    ///
    bool isDefault() const;
    ///
    operator TValue() const;
    ///
    NodeView &operator= (TValue const &value);
    ///
    bool operator== (TValue const &value) const;
    ///
    INode::TSharedPtr getNode() const;

protected:
    /// list attribute
    NodeView(
        INode::TSharedPtr       const &node,
        std::string             const &name,
        std::string             const &path);

    NodeView(std::string const &name);
    /// attribute
    NodeView(NodeView<void> const * const parent, std::string const &name, TValue const &default_value);
    /// attribute
    NodeView(NodeView<void> const * const parent, std::string const &name);

    INode::TSharedPtr       m_node;
    std::string             m_path;
    bool                    m_is_default = false;
};


template<>
class NodeView<void> {
public:
    /// root node for desiarization only
    NodeView(std::string const &name);
    /// root node for serialization and desiarization
    NodeView(
         INode::TSharedPtr  const &node,
         std::string        const &name);
    ///
   ~NodeView() = default;
    ///
    INode::TSharedPtr       getNode() const;
    ///
    INode::TSharedPtr       m_node;
    ///
    std::string             m_path;

protected:
    /// list node
    NodeView(
        INode::TSharedPtr   const &node,
        std::string         const &name,
        std::string         const &path);
    /// node
    NodeView(NodeView<void> const * const parent, std::string const &name);
};


template<typename TNodeView>
class NodeViewList {
public:
    ///
   ~NodeViewList() = default;
    ///
    typedef typename std::list<TNodeView> TNodes;
    typedef typename std::list<TNodeView>::iterator iterator;
    typedef typename std::list<TNodeView>::const_iterator const_iterator;
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
    void add(TNodeView const &node);
    ///
    template<typename TValue>
    void add(TValue const &node);

protected:
    ///
    NodeViewList(NodeView<void> const * const parent, std::string const &name);

private:
    ///
    NodeView<void> const * const m_parent;
    ///
    std::list<TNodeView> m_nodes;
};


template<typename TNodeView>
class NodeViewPtr {
public:
    NodeViewPtr(NodeView<void> const * const parent);
   ~NodeViewPtr() = default;
    std::shared_ptr<TNodeView> get() const;

private:
    typename std::shared_ptr<TNodeView> m_node_view;
};


// ----- implementation


// ----- Node


template<typename TValue>
NodeView<TValue>::NodeView(
    INode::TSharedPtr   const &node,
    std::string         const &name,
    std::string         const &path)
{
    m_path = path + "/" + name;

    if (node && node->getName() == name)
        m_node   = node;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
}


template<typename TValue>
NodeView<TValue>::NodeView(std::string const &name) {
    m_path = "/" + name;
    m_node = implementation::CNode::create(name);
}


template<typename TValue>
INode::TSharedPtr NodeView<TValue>::getNode() const {
    return m_node; // ----->
}


template<typename TValue>
NodeView<TValue>::NodeView(
    NodeView<void>  const * const parent,
    std::string     const &name,
    TValue          const &default_value)
{
    m_path = parent->m_path + "/" + name;

    if (parent->m_node)
        m_node          = parent->m_node->getChild(name);
    else
        m_node          = nullptr;

    if(!m_node) {
        m_node          = parent->m_node->addChild(name, convertion::convert<std::string>(default_value));
        m_is_default    = true;
    }
}


template<typename TValue>
NodeView<TValue>::NodeView(NodeView<void> const * const parent, std::string const &name) {
    m_path = parent->m_path + "/" + name;

    if (parent->m_node)
        m_node = parent->m_node->getChild(name);
    else
        m_node = nullptr; // pvs warn

    if(!m_node) {
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
    }
}


template<typename TValue>
TValue NodeView<TValue>::get() const {
    // todo: lazy convertion with cached value
    if (m_node)
        return convertion::convert<TValue>(m_node->getValue());
    else
        throw std::runtime_error("value " + m_path + " not serialized"); // ----->
}


template<typename TValue>
void NodeView<TValue>::set(TValue const &value) {
    if (m_node)
        m_node->setValue(convertion::convert<std::string>(value));

    m_is_default = false;
}


template<typename TValue>
bool NodeView<TValue>::isDefault() const {
    return m_is_default;
}


template<typename TValue>
NodeView<TValue>::operator TValue() const {
    return get(); // ----->
}


template<typename TValue>
NodeView<TValue> &NodeView<TValue>::operator= (TValue const &value) {
    set(value);
    return *this; // ----->
}


template<typename TValue>
bool NodeView<TValue>::operator== (TValue const &value) const {
    return get() == value; // ----->
}
// -----


// ----- NodeViewList


template<typename TNodeView>
NodeViewList<TNodeView>::NodeViewList(NodeView<void> const * const parent, std::string const &name)
:
    m_parent(parent)
{
    if (parent->m_node) {
        for (auto const &i: *parent->m_node)
            if (i->getName() == name)
                m_nodes.push_back(TNodeView(i, parent->m_path));
    }
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::iterator NodeViewList<TNodeView>::begin() {
    return m_nodes.begin(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::iterator NodeViewList<TNodeView>::end() {
    return m_nodes.end(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::const_iterator NodeViewList<TNodeView>::begin() const {
    return m_nodes.begin(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::const_iterator NodeViewList<TNodeView>::end() const {
    return m_nodes.end(); // ----->
}


template<typename TNodeView>
size_t NodeViewList<TNodeView>::size() const {
    return m_nodes.size(); // ----->
}


template<typename TNodeView>
void NodeViewList<TNodeView>::add(TNodeView const &node) {
    m_parent->m_node->addChild(node.getNode());
    m_nodes.push_back(node);
}


template<typename TNodeView>
template<typename TValue>
void NodeViewList<TNodeView>::add(TValue const &value) {
    TNodeView node_view;
    node_view.set(value);
    add(node_view);
}
// -----


// ----- NodeList


template<typename TNodeView>
NodeViewPtr<TNodeView>::NodeViewPtr(NodeView<void> const * const parent) {
    try {
        m_node_view = std::make_shared<TNodeView>(parent);
    } catch (...) {}
}


template<typename TNodeView>
std::shared_ptr<TNodeView> NodeViewPtr<TNodeView>::get() const {
    return m_node_view; // ----->
}
// -----


std::string convertCamelToDelimitedBySymbol(std::string const &camel, char const &delimeter_symbol);


} // serialization
} // parsing
} // iridium

// todo: name_delimeter_symbol -> name_convertion_method
#define DEFINE_ROOT_NODE_BEGIN_2(class_name, name_delimeter_symbol) \
    struct T##class_name : protected iridium::parsing::serialization::NodeView<void> { \
        static int const NAME_DELIMETER_SYMBOL = name_delimeter_symbol; \
        T##class_name(iridium::parsing::INode::TSharedPtr const &node): \
            iridium::parsing::serialization::NodeView<void> \
                (node, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
            T##class_name(): iridium::parsing::serialization::NodeView<void> \
                (iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
            iridium::parsing::INode::TSharedPtr getNode() const { \
                return iridium::parsing::serialization::NodeView<void>::getNode(); \
            } \
        T##class_name(iridium::parsing::serialization::NodeView<void> const * const parent): \
            iridium::parsing::serialization::NodeView<void> \
                (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {}


#define DEFINE_ROOT_NODE_BEGIN_1(class_name) \
    DEFINE_ROOT_NODE_BEGIN_2(class_name, '-')


#define DEFINE_ROOT_NODE_BEGIN(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_ROOT_NODE_BEGIN, __VA_ARGS__)(__VA_ARGS__)


#define DEFINE_ROOT_NODE_END() \
    };


#define DEFINE_NODE_BEGIN(class_name) \
    struct T##class_name : protected iridium::parsing::serialization::NodeView<void> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const * const parent): \
        iridium::parsing::serialization::NodeView<void> \
        (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {}


#define DEFINE_NODE_END(class_name) \
    } class_name = this;


#define DEFINE_ATTRIBUTE_2(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const * const parent): \
        iridium::parsing::serialization::NodeView<type> \
        (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
    } class_name = this;


#define DEFINE_ATTRIBUTE_3(type, class_name, default_value) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const * const parent): \
        iridium::parsing::serialization::NodeView<type> \
        (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), default_value) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
    } class_name = this;


#define DEFINE_ATTRIBUTE(...) \
DEFINE_MACRO_CHOOSER(DEFINE_ATTRIBUTE, __VA_ARGS__)(__VA_ARGS__)


// todo: bugfix node name, e g NameList instead Name
#define DEFINE_NODE_LIST_BEGIN(class_name) \
    struct T##class_name##List; \
    struct T##class_name: public iridium::parsing::serialization::NodeView<void> { \
        T##class_name( \
        iridium::parsing::INode::TSharedPtr const &node, \
        std::string const &path): \
        iridium::parsing::serialization::NodeView<void> \
        (node, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), path) {} \
        T##class_name(): iridium::parsing::serialization::NodeView<void> \
        (iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {}


#define DEFINE_NODE_LIST_END(class_name) \
    }; \
    struct T##class_name##List: public iridium::parsing::serialization::NodeViewList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::NodeView<void> const * const parent): \
        iridium::parsing::serialization::NodeViewList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    } class_name = this;


#define DEFINE_ATTRIBUTE_LIST(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const * const parent): \
            iridium::parsing::serialization::NodeView<type> \
                (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
        T##class_name( \
            iridium::parsing::INode::TSharedPtr const &node, \
            std::string const &path): \
                iridium::parsing::serialization::NodeView<type> \
                    (node, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), path) {} \
        T##class_name(): \
            iridium::parsing::serialization::NodeView<type> \
                (iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    }; \
    struct T##class_name##List : public iridium::parsing::serialization::NodeViewList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::NodeView<void> const * const parent): \
        iridium::parsing::serialization::NodeViewList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToDelimitedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    } class_name = this;


#define DEFINE_NODE_EXTERNAL(class_name) \
    T##class_name class_name = this;


// recursive node
#define DEFINE_NODE_PTR(class_name) \
    typedef typename iridium::parsing::serialization::NodeViewPtr<T##class_name> T##class_name##Ptr; \
    T##class_name##Ptr class_name##_ptr = this;


// recursive node pointer
#define DEFINE_NODE_LIST_PTR(class_name) \
    typedef typename iridium::parsing::serialization::NodeViewPtr<T##class_name##List> T##class_name##ListPtr; \
    T##class_name##ListPtr class_name##_list_ptr = this;


#endif // HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
