#ifndef HEADER_PARSER_C9F03CE0_FD15_4D1F_B9C7_AA9C3201A348
#define HEADER_PARSER_C9F03CE0_FD15_4D1F_B9C7_AA9C3201A348


#include <string>

#include "utility/smart_ptr.h"
#include "node.h"


namespace utility {
namespace parsing {


class IParser {
public:
    DEFINE_SMART_PTR(IParser);
    ///
    virtual ~IParser() = default;
    ///
    virtual INode::TSharedPtr getRootNode() const = 0;
    ///
    virtual void setRootNode(INode::TSharedPtr const &root_node) = 0;
    ///
    virtual void parse(std::string const &source) = 0;
    ///
    virtual std::string compose() const = 0;
};


} // parsing
} // utility


#endif // HEADER_PARSER_C9F03CE0_FD15_4D1F_B9C7_AA9C3201A348
