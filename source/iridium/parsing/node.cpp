#include "node.h"


using std::string;


namespace {


//string convertNodeToString(iridium::parsing::INode::TConstSharedPtr const &node, string const &tab = "") {
//    if (!node)
//        return "nullptr"; // ----->
//    string result = "\n" + tab + "'" + node->getName() + "'" + " = " + "'" + node->getValue() + "'";
//    for (auto const &i : *node)
//        result += convertNodeToString(i, tab + "  ");
//    return result; // ----->
//}


string convertNodeToString(iridium::parsing::INode const &node, string const &tab = "") {
    string result = "\n" + tab + "'" + node.getName() + "'" + " = " + "'" + node.getValue() + "'";
    for (auto const &i : node)
        result += convertNodeToString(*i, tab + "  ");
    return result; // ----->
}


} // unnamed


//IMPLEMENT_CONVERT(std::string, iridium::parsing::INode::TConstSharedPtr , convertNodeToString)
//IMPLEMENT_CONVERT(std::string, iridium::parsing::INode::TSharedPtr      , convertNodeToString)
IMPLEMENT_CONVERT(std::string, iridium::parsing::INode, convertNodeToString)
