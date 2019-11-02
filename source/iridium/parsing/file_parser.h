// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
#define HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896


#include <string>

#include "node.h"

#include "iridium/smart_ptr.h"


namespace iridium {
namespace parsing {


///
INode::TSharedPtr parseFile(std::string const &file_name);
///
void composeFile(std::string const &file_name, INode::TSharedPtr const &root_node);


} // parser
} // iridium


#endif // HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
