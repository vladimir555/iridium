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
class CHTTPRequestParser: public IParser {
public:
    DEFINE_CREATE(CHTTPRequestParser)
    CHTTPRequestParser() = default;
    virtual ~CHTTPRequestParser() = default;

    INode::TSharedPtr parse(std::string const &source) const override;
    std::string compose(INode::TConstSharedPtr const &root_node, INode::TConstSharedPtr const &root_node_types = nullptr) const override;
};


} // implementation
} // parser
} // utility


#endif // HEADER_PARSER_HTTP_REQUEST_DC9EBA78_0C84_4F8B_A3B4_992A8B6E7BF3
