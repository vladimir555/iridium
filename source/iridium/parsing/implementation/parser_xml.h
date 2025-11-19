// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
#define HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium::parsing::implementation {


class CXMLParser: public IParser {
public:
    DEFINE_IMPLEMENTATION(CXMLParser)
    CXMLParser() = default;

    INode::TSharedPtr parse(std::string const &source) const override;
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // iridium::parsing::implementation


#endif // HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
