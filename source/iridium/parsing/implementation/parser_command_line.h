// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_COMMAND_LINE_BD6035D1_9318_4137_8F01_90F9EA796AF2
#define HEADER_PARSER_COMMAND_LINE_BD6035D1_9318_4137_8F01_90F9EA796AF2


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace parsing {
namespace implementation {


class CCommandLineParser: public IParser {
public:
    DEFINE_IMPLEMENTATION(CCommandLineParser)
    CCommandLineParser() = default;
    INode::TSharedPtr parse(std::string const &source) const override;
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parsing
} // iridium


#endif // HEADER_PARSER_COMMAND_LINE_BD6035D1_9318_4137_8F01_90F9EA796AF2
