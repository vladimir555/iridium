#ifndef HEADER_PARSER_7B41E537_8C35_413A_90A5_BD7A36FA095F
#define HEADER_PARSER_7B41E537_8C35_413A_90A5_BD7A36FA095F


#include "utility/pasring/node.h"


namespace utility {
namespace parsing {
namespace implementation {


class CParser {
public:
    ///
    CParser() = default;
    ///
    ~CParser() = default;
    ///
    virtual INode::TSharedPtr getRootNode();
    ///
    virtual void setRootNode(INode::TSharedPtr const &root_node);

protected:
    ///
    INode::TSharedPtr m_root_node;
};


} // implementation
} // parsing
} // utility


#endif // HEADER_PARSER_7B41E537_8C35_413A_90A5_BD7A36FA095F
