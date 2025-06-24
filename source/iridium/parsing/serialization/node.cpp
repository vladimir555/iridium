#include "node.h"

#include "iridium/parsing/implementation/node.h"

#include <stdexcept>


using std::string;
using iridium::parsing::implementation::CNode;


namespace iridium {
namespace parsing {
namespace serialization {


NodeView<void>::NodeView(std::string const &name){
    m_path = "/" + name;
    m_node = CNode::create(name);
}


NodeView<void>::NodeView(
    INode::TSharedPtr   const &node,
    std::string         const &name)
{
    m_path = "/" + name;

    if (node && node->getName() == name)
        m_node = node;
    else
        m_node = CNode::create(name);
}


NodeView<void>::NodeView(
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


NodeView<void>::NodeView(NodeView<void> const * const parent, std::string const &name) {
    if (!parent)
        throw std::runtime_error(
            "node name '" + name + "' add to path '" + m_path + "' error: parent is null"); // ----->

    m_path = parent->m_path + "/" + name;

    if (parent && parent->m_node)
        m_node = parent->m_node->getChild(name);

    // add empty node for child nodes with default values
    if(!m_node)
        m_node = parent->m_node->addChild(name);
}


INode::TSharedPtr NodeView<void>::getNode() const {
    return m_node; // ----->
}


std::string convertCamelToDelimitedBySymbol(std::string const &camel, char const &delimiter_symbol) {
    if (camel.empty() || delimiter_symbol == 0)
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
