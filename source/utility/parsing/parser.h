#ifndef HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
#define HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880


#include <string>

#include "utility/smart_ptr.h"

#include "node.h"


namespace utility {
namespace parsing {


///
class IParser {
public:
    DEFINE_SMART_PTR(IParser)
    ///
    virtual ~IParser() = default;
    ///
    virtual INode::TSharedPtr parse(std::string const &source) const = 0;
    ///
    virtual std::string compose(INode::TConstSharedPtr const &root_node, INode::TConstSharedPtr const &root_node_types = nullptr) const = 0;
};


} // parser
} // utility


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
