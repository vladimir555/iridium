// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
#define HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace parsing {
namespace implementation {


///
class CJSONParser : public IParser {
public:
    DEFINE_IMPLEMENTATION(CJSONParser)
    ///
    CJSONParser() = default;
    ///
    INode::TSharedPtr parse(std::string const &source) const override;
    ///
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parser
} // iridium


#endif // HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
