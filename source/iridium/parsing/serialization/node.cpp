/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "node.h"

#include "iridium/parsing/implementation/node.h"


using std::string;
using iridium::parsing::implementation::CNode;


namespace iridium {
namespace parsing {
namespace serialization {


Node<void>::Node(std::string const &name){
    m_path              = "/" + name;
    m_node_destination  = CNode::create(name);
}


Node<void>::Node(INode::TConstSharedPtr const &node, std::string const &name) {
    m_path = "/" + name;

    if (node && node->getName() == name)
        m_node_source  = node;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->

    m_node_destination = CNode::create(name);
}


Node<void>::Node(
    INode::TConstSharedPtr  const &node_source,
    INode::TSharedPtr       const &node_destination,
    std::string             const &name,
    std::string             const &path)
{
    m_path = path + "/" + name;

    if (node_source && node_source->getName() == name)
        m_node_source       = node_source;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->

    m_node_destination = node_destination;
}


Node<void>::Node(Node<void> const &parent, std::string const &name) {
    m_path = parent.m_path + "/" + name;

    if (parent.m_node_source)
        m_node_source       = parent.m_node_source->getChild(name);

    if (parent.m_node_destination)
        m_node_destination  = parent.m_node_destination->addChild(name);
}


INode::TSharedPtr Node<void>::getNode() const {
    return m_node_destination; // ----->
}


string convertCamelToDashed(std::string const &camel) {
    //todo: optimize
    string result = camel;

    if (!result.empty())
        result[0] = static_cast<char>(tolower(result[0]));

    size_t i = 1;
    while (i < result.size()) {
        char lo_ch = static_cast<char>(tolower(result[i]));
        if (result[i] != lo_ch) {
            result[i] = static_cast<char>(lo_ch);
            result.insert(i, "-");
        }
        i++;
    }

    return result; // ----->
}


} // serialization
} // parsing
} // iridium
