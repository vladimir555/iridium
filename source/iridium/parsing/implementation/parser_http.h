// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616
#define HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace parsing {
namespace implementation {


class CHTTPParser: public IParser {
public:
    DEFINE_IMPLEMENTATION(CHTTPParser)
    CHTTPParser() = default;
    INode::TSharedPtr parse(std::string const &source) const override;
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parser
} // iridium


#endif // HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616
