#ifndef HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
#define HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A


#include "utility/smart_ptr.h"
#include "utility/parsing/node.h"
#include "utility/parsing/parser.h"


namespace utility {
namespace parsing {
namespace implementation {


///
class CXMLParser: public IParser {
public:
    DEFINE_CREATE(CXMLParser)
    ///
    CXMLParser() = default;
    ///
    virtual ~CXMLParser() = default;
    ///
    virtual INode::TSharedPtr parse(std::string const &source) const override;
    ///
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parser
} // utility


#endif // HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
