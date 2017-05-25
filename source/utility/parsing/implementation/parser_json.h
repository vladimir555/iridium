#ifndef HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
#define HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88


#include "utility/smart_ptr.h"
#include "utility/parsing/node.h"
#include "utility/parsing/parser.h"


namespace utility {
namespace parsing {
namespace implementation {


///
class CJSONParser : public IParser {
public:
    DEFINE_CREATE(CJSONParser)
    ///
    CJSONParser() = default;
    ///
    virtual ~CJSONParser() = default;
    ///
    virtual INode::TSharedPtr parse(std::string const &source) const override;
    ///
    virtual std::string compose(INode::TConstSharedPtr const &root_node, INode::TConstSharedPtr const &root_node_types = nullptr) const override;
};


} // implementation
} // parser
} // utility


#endif // HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
