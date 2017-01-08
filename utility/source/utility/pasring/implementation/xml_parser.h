#ifndef HEADER_XML_PARSER_D8079B34_6A8A_4400_870C_0475EA306A2D
#define HEADER_XML_PARSER_D8079B34_6A8A_4400_870C_0475EA306A2D


#include "utility/smart_ptr.h"
#include "utility/pasring/node.h"
#include "utility/pasring/parser.h"

#include "parser.h"


namespace utility {
namespace parsing {
namespace implementation {


///
class CXMLParser:
    public IParser,
    public CParser
{
public:
    DEFINE_SMART_PTR(CXMLParser);
    ///
    CXMLParser() = default;
    ///
    virtual ~CXMLParser() = default;
    ///
    virtual void parse(std::string const &source);
    ///
    virtual std::string compose() const;
};


} // implementation
} // parsing
} // utility


#endif // HEADER_XML_PARSER_D8079B34_6A8A_4400_870C_0475EA306A2D
