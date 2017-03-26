#ifndef HEADER_NODE_7292FAED_D43F_4ED3_A127_6C80D3FFD559
#define HEADER_NODE_7292FAED_D43F_4ED3_A127_6C80D3FFD559


#include "utility/pasring/node.h"


namespace utility {
namespace parsing {
namespace serialization {


// ----- interface


///
class Node {
public:
    ///
    Node(Node * const parent, std::string const &name, std::string const &default_value);
    ///
    Node(INode::TSharedPtr const &node, std::string const &name);
    ///
    Node(Node * const parent, std::string const &name);
    ///
    template<typename TNode>
    void makeNodeList(std::string const &name, std::list<TNode> &node_list);
    ///
    ~Node() = default;
    ///
    static std::string convertCamelToDashed(std::string const &camel);

private:
    ///
    INode::TSharedPtr m_node;
    ///
    template<typename T>
    friend class Attribute;
};


template<typename TValue>
class Attribute: public Node {
public:
    ///
    explicit Attribute(Node * const parent, std::string const &name, TValue const &default_value);
    ///
    explicit Attribute(Node * const parent, std::string const &name);
    ///
    Attribute(TValue const &) = default;
    ///
    ~Attribute() = default;
    ///
    void operator = (TValue const &v);
    ///
    operator TValue() const;
};


template<typename TNode>
class NodeList {
public:
    ///
    NodeList(Node * const parent, std::string const &name);
    ///
    typedef typename std::list<TNode>                   TNodes;
    typedef typename std::list<TNode>::iterator         iterator;
    typedef typename std::list<TNode>::const_iterator   const_iterator;
    ///
    iterator begin();
    ///
    iterator end();
    ///
    const_iterator begin() const;
    ///
    const_iterator end() const;

private:
    ///
    TNodes m_nodes;
};


// ----- implementation


// -----Node
template<typename TNode>
void Node::makeNodeList(std::string const &name, std::list<TNode> &node_list) {
    for (auto &child: *m_node)
        if (child->getName() == name)
            node_list.push_back(TNode(child, name));
}
// -----


// ----- Attribute
template<typename TValue>
Attribute<TValue>::Attribute(Node * const parent, std::string const &name, TValue const &default_value)
:
    Node(parent, name, convertion::convert<std::string>(TValue(default_value)))
{}


template<typename TValue>
Attribute<TValue>::Attribute(Node * const parent, std::string const &name)
:
    Node(parent, name, "")
{}


template<typename TValue>
void Attribute<TValue>::operator = (TValue const &v) {
    m_node->setValue(convertion::convert<std::string>(v));
}


template<typename TValue>
Attribute<TValue>::operator TValue() const {
    return TValue(convertion::convert<TValue>(m_node->getValue())); // ----->
}
// -----


// ----- NodeList
template<typename TValue>
NodeList<TNode>::NodeList(Node * const parent, std::string const &name) {
    parent->makeNodeList(name, m_nodes);
}


template<typename TValue>
typename NodeList<TNode>::iterator NodeList<TNode>::begin() {
    return m_nodes.begin(); // ----->
}


template<typename TValue>
typename NodeList<TNode>::iterator NodeList<TNode>::end() {
    return m_nodes.end(); // ----->
}


template<typename TValue>
typename NodeList<TNode>::const_iterator NodeList<TNode>::begin() const {
    return m_nodes.begin(); // ----->
}


template<typename TValue>
typename NodeList<TNode>::const_iterator NodeList<TNode>::end() const {
    return m_nodes.end(); // ----->
}
// -----


} // serialization
} // parsing
} // utility


#define DEFINE_ROOT_NODE_BEGIN(class_name) \
struct T##class_name: public utility::parsing::serialization::Node { \
    T##class_name(utility::parsing::INode::TSharedPtr const &node): \
        Node(node, utility::parsing::serialization::Node::convertCamelToDashed(#class_name)) \
    {} \
    protected: \
        T##class_name(utility::parsing::INode::TSharedPtr const &node, std::string const &class_name_ext): \
            utility::parsing::serialization::Node(node, utility::parsing::serialization::Node::convertCamelToDashed(#class_name_ext)) \
        {} \
    public:

#define ROOT_NODE_BEGIN_EXT(class_name, class_name_ext) \
    struct T##class_name: public T##class_Name_ext { \
        T##class_name(utility::parsing::INode::TSharedPtr const &node): \
            T##class_name_ext(node, #class_name) \
        {}


#endif // HEADER_NODE_7292FAED_D43F_4ED3_A127_6C80D3FFD559
