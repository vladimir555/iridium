#ifndef HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3
#define HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"

#include <string>


namespace iridium {
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
} // iridium


#endif // HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3
