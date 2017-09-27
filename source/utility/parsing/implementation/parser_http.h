#ifndef HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3
#define HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3


#include "utility/smart_ptr.h"
#include "utility/parsing/node.h"
#include "utility/parsing/parser.h"

#include <string>


namespace utility {
namespace parsing {
namespace implementation {


//todo: http request / responce
class CHTTPParser: public IParser {
public:
    DEFINE_CREATE(CHTTPParser)
    CHTTPParser() = default;
    virtual ~CHTTPParser() = default;

    INode::TSharedPtr parse(std::string const &source) const override;
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parser
} // utility


#endif // HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3
