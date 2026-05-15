// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
#define HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880


#include <string>

#include "iridium/smart_ptr.h"

#include "node.h"


namespace iridium::parsing {


class IParser {
public:
    DEFINE_SMART_PTR(IParser)
    virtual ~IParser() = default;

    virtual INode::TSharedPtr parse(std::string const &source) const = 0;
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const = 0;
};


} // namespace iridium::parsing


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
