#include "node.h"


using std::string;


namespace utility {
namespace parsing {


string const INode::PATH_DELIMITER = "/";


} // parsing
} // utility


namespace {


string convertNodeToString(utility::parsing::INode::TConstSharedPtr const &node, string const &tab = "") {
    string result = tab + "'" + node->getName() + "'" + " = " + node->getValue() + "'";
    for (auto const &i: *node)
        result += convertNodeToString(i, tab + "  ");
    return result; // ----->
}


}


namespace utility {
namespace convertion {
namespace implementation {


template<>
string convert(utility::parsing::INode::TSharedPtr const &node) {
    return convertNodeToString(node); // ----->
}


} // implementation
} // convertion
} // utility
