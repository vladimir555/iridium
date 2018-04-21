#include "node.h"



using std::string;


namespace {


string convertNodeToString(utility::parsing::INode::TConstSharedPtr const &node, string const &tab = "") {
    if (!node)
        return "NULL"; // ----->
    string result = "\n" + tab + "'" + node->getName() + "'" + " = " + "'" + node->getValue() + "'";
    for (auto const &i : *node)
        result += convertNodeToString(i, tab + "  ");
    return result; // ----->
}


} // unnamed


IMPLEMENT_CONVERT(std::string, utility::parsing::INode::TConstSharedPtr, convertNodeToString)
IMPLEMENT_CONVERT(std::string, utility::parsing::INode::TSharedPtr, convertNodeToString)
