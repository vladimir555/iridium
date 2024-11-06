// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
#define  HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3


#include "node_type.h"


namespace iridium {
namespace parsing {


typedef INodeType<std::string> INode;


} // parsing
} // iridium


DEFINE_CONVERT(std::string, iridium::parsing::INode)


#endif // HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
