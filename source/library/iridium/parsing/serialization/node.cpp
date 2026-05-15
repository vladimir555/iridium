#include "node.h"

#include "iridium/parsing/implementation/node.h"

#include <stdexcept>


using std::string;
using iridium::parsing::implementation::CNode;


namespace iridium::parsing::serialization {


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
    return m_node->clone(); // ----->
}


std::string convertCamelToDelimitedBySymbol(std::string &&camel, char delimiter_symbol) {
    if (camel.empty() || delimiter_symbol == '\0') {
        return std::move(camel);
    }

    std::string result;
    result.reserve(camel.size() + 16);
    result.push_back(static_cast<char>(std::tolower(camel[0])));

    for (size_t i = 1; i < camel.size(); ++i) {
        if (std::isupper(camel[i])) {
            result.push_back(delimiter_symbol);
            result.push_back(static_cast<char>(std::tolower(camel[i])));
        } else {
            result.push_back(camel[i]);
        }
    }

    return result; // ----->
}


std::string convertNameCPPToNode(std::string &&name, TNamingStrategyCPPToNode const &strategy) {
    switch (strategy) {
        case TNamingStrategyCPPToNode::CAMEL_CASE_TO_LOWER_KEBAB:
            return convertCamelToDelimitedBySymbol(std::move(name), '-');
        case TNamingStrategyCPPToNode::LOWER_CASE_TO_LOWER_KEBAB:
            std::replace(name.begin(), name.end(), '_', '-');
            return std::move(name);
        default:
            break;
    }
    return std::move(name); // ----->
}


} // iridium::parsing::serialization


//IMPLEMENT_ENUM(iridium::parsing::serialization::TNamingStrategyCPPToNode)
