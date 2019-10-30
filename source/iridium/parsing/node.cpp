/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "node.h"



using std::string;


namespace {


string convertNodeToString(iridium::parsing::INode::TConstSharedPtr const &node, string const &tab = "") {
    if (!node)
        return "NULL"; // ----->
    string result = "\n" + tab + "'" + node->getName() + "'" + " = " + "'" + node->getValue() + "'";
    for (auto const &i : *node)
        result += convertNodeToString(i, tab + "  ");
    return result; // ----->
}


} // unnamed


IMPLEMENT_CONVERT(std::string, iridium::parsing::INode::TConstSharedPtr, convertNodeToString)
IMPLEMENT_CONVERT(std::string, iridium::parsing::INode::TSharedPtr, convertNodeToString)
