#ifndef HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
#define HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3


#include "node_type.h"


namespace utility {
namespace parsing {


typedef INodeType<std::string> INode;


} // parsing
} // utility


DEFINE_CONVERT(std::string, utility::parsing::INode::TConstSharedPtr)
DEFINE_CONVERT(std::string, utility::parsing::INode::TSharedPtr)


#endif // HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
