#ifndef HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2
#define HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2


#include "utility/smart_ptr.h"
#include "utility/pasring/node.h"

#include "parser.h"


namespace utility {
namespace parsing {
namespace implementation {


class CJSONParser: public CParser {
public:
    DEFINE_CREATE(CJSONParser)
    ///
    CJSONParser() = default;
    ///
    virtual ~CJSONParser() = default;
    ///
    virtual void parse(std::string const &source) override;
    ///
    virtual std::string compose() const override;
};


} // implementation
} // parsing
} // utility


#endif // HEADER_JSON_PARSER_8A41857D_C5F4_446B_8C1A_FF3BA775A2E2
