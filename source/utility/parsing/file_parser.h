#ifndef HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
#define HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896


#include <string>

#include "node.h"

#include "utility/smart_ptr.h"


namespace utility {
namespace parsing {


///
INode::TSharedPtr parseFile(std::string const &file_name);
///
void composeFile(std::string const &file_name, INode::TSharedPtr const &root_node);


} // parser
} // utility


#endif // HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
