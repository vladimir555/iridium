#include "node.h"

#include "iridium/parsing/implementation/node.h"

#include <stdexcept>


using std::string;
using iridium::parsing::implementation::CNode;


namespace iridium {
namespace parsing {
namespace serialization {


Node<void>::Node(std::string const &name){
    m_path = "/" + name;
    m_node = CNode::create(name);
}


Node<void>::Node(
    INode::TSharedPtr   const &node,
    std::string         const &name)
{
    m_path = "/" + name;

    if (node && node->getName() == name)
        m_node = node;
    else
        m_node = CNode::create(name);
}


Node<void>::Node(
    INode::TSharedPtr   const &node,
    std::string         const &name,
    std::string         const &path)
{
    m_path = path + "/" + name;

    if (node && node->getName() == name)
        m_node = node;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
}


Node<void>::Node(Node<void> const &parent, std::string const &name) {
    m_path = parent.m_path + "/" + name;

    if (parent.m_node)
        m_node = parent.m_node->getChild(name);

    // add empty node for child nodes with default values
    if(!m_node)
        m_node = parent.m_node->addChild(name);
}


INode::TSharedPtr Node<void>::getNode() const {
    return m_node; // ----->
}


//string convertCamelToSplittedBySymbol(std::string const &camel, char const &delimeter_symbol) {
//    //todo: optimize
//    string result = camel;
//
//    if (!result.empty())
//        result[0] = static_cast<char>(tolower(result[0]));
//
//    size_t i = 1;
//    while (i < result.size()) {
//        char lo_ch = static_cast<char>(tolower(result[i]));
//        if (result[i] != lo_ch) {
//            result[i] = static_cast<char>(lo_ch);
//            result.insert(i, string() + delimeter_symbol);
//        }
//        i++;
//    }
//
//    return result; // ----->
//}


std::string convertCamelToSplittedBySymbol(std::string const &camel, char const &delimiter_symbol) {
    if (camel.empty())
        return camel;

    std::string result;

    result.reserve(camel.size() * 2);
    result += static_cast<char>(std::tolower(camel[0]));

    for (size_t i = 1; i < camel.size(); ++i) {
        if (std::isupper(camel[i])) {
            result += delimiter_symbol;
            result += static_cast<char>(std::tolower(camel[i]));
        } else
            result += camel[i];
    }

    return result; // ----->
}


} // serialization
} // parsing
} // iridium
