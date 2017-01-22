#ifndef HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2
#define HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2


#include "utility/smart_ptr.h"
#include "utility/pasring/node.h"
#include "utility/pasring/parser.h"

#include "parser.h"


namespace utility {
namespace parsing {
namespace implementation {


class CJSONParser:
    public IParser,
    public CParser
{
public:
    DEFINE_SMART_PTR(CJSONParser);
    ///
    CJSONParser() = default;
    ///
    virtual ~CJSONParser() = default;
    ///
    virtual void parse(std::string const &source);
    ///
    virtual std::string compose() const;
};


} // implementation
} // parsing
} // utility


#endif // HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2
