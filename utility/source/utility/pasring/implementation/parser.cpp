#include "parser.h"



namespace utility {
namespace parsing {
namespace implementation {


INode::TSharedPtr CParser::getRootNode() const {
    return m_root_node;
}


void CParser::setRootNode(INode::TSharedPtr const &root_node) {
    m_root_node = root_node;
}


} // implementation
} // parsing
} // utility
