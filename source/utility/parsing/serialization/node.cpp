#include "node.h"

#include "utility/parsing/implementation/node.h"


using std::string;
using utility::parsing::implementation::CNode;


namespace utility {
namespace parsing {
namespace serialization {


Node<void>::Node(INode::TConstSharedPtr const &node, std::string const &name, std::string const &path) {
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
}


string convertCamelToDashed(std::string const &camel) {
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
} // utility
