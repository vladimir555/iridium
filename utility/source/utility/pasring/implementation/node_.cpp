#include "node.h"


using std::string;


namespace utility {
namespace parsing {
namespace implementation {


CNode::CNode(string const &name, string const &value)
:
    m_name  (name),
    m_value (value)
{}


CNode::CNode(string const &name)
:
    m_name(name)
{}


string CNode::getName() const {
    return m_name; // ----->
}


string CNode::getValue() const {
    return m_value; // ----->
}


void CNode::setValue(string const &value) {
    m_value = value;
}


INode::TNodes CNode::findChilds(string const &path) {
    INode::TNodes nodes;

    if (path.empty())
        return nodes; // ----->

    string child_path = path.substr(1, path.find(INode::PATH_DELIMITER, 1) - 1);
    string next_path;

    if ((child_path.size() + INode::PATH_DELIMITER.size()) <= path.size())
        next_path = path.substr(child_path.size() + INode::PATH_DELIMITER.size());

    if (next_path.empty()) {
        for (auto const &node : m_nodes)
            if (node->getName() == child_path)
                nodes.push_back(node);
    } else {
        for (auto const &node : m_nodes)
            if (node->getName() == child_path)
                nodes.splice(nodes.end(), node->findChilds(next_path));
    }

    return nodes; // ----->
}


INode::iterator CNode::begin() {
    return m_nodes.begin(); // ----->
}


INode::iterator CNode::end() {
    return m_nodes.end(); // ----->
}


INode::const_iterator CNode::begin() const {
    return m_nodes.begin(); // ----->
}


INode::const_iterator CNode::end() const {
    return m_nodes.end(); // ----->
}


size_t CNode::size() const {
    return m_nodes.size(); // ----->
}


bool CNode::hasChilds() const {
    return !m_nodes.empty(); // ----->
}


INode::TSharedPtr CNode::addChild(INode::TSharedPtr const &child_node) {
    m_nodes.push_back(child_node);
    return m_nodes.back(); // ----->
}


INode::TSharedPtr CNode::addChild(string const &name) {
    auto node = CNode::create(name);
    return addChild(node); // ----->
}


INode::TSharedPtr CNode::addChild(string const &name, string const &value) {
    auto node = CNode::create(name, value);
    return addChild(node); // ----->
}


INode::TSharedPtr CNode::clone() const {
    return TSharedPtr();
}


} // implementation
} // parsing
} // utility
