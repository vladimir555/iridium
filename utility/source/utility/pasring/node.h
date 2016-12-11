#ifndef HEADER_NODE_BA28CA42_211E_44C4_BA4F_4CF3BD0B9B34
#define HEADER_NODE_BA28CA42_211E_44C4_BA4F_4CF3BD0B9B34


#include <string>
#include <list>

#include "utility/smart_ptr.h"
#include "utility/convertion/convert.h"


namespace utility {
namespace parsing {


class INode {
public:
    DEFINE_SMART_PTR(INode);
    ///
    virtual ~INode() = default;
    ///
    typedef std::list<INode::TSharedPtr> TNodes;
    ///
    typedef TNodes::iterator iterator;
    ///
    typedef TNodes::const_iterator const_iterator;
    ///
    typedef TNodes::const_reference const_reference;
    ///
    typedef TNodes::const_reverse_iterator const_reverse_iterator;
    ///
    virtual std::string getName() const = 0;
    ///
    virtual std::string getValue() const = 0;
    ///
    virtual void setValue(std::string const &value) = 0;
    ///
    virtual INode::TNodes findChilds(std::string const &path) = 0;
    ///
    virtual iterator begin() = 0;
    ///
    virtual iterator end() = 0;
    ///
    virtual const_iterator begin() const = 0;
    ///
    virtual const_iterator end() const = 0;
    ///
    virtual bool hasChilds() const = 0;
    ///
    virtual INode::TSharedPtr addChilds() const = 0;
    ///
    virtual INode::TSharedPtr addChild(INode::TSharedPtr const &child_node) = 0;
    ///
    virtual INode::TSharedPtr addChild(std::string const &name) = 0;
    ///
    virtual INode::TSharedPtr addChild(std::string const &name, std::string const &value) = 0;
    ///
    virtual INode::TConstSharedPtr clone() const = 0;
    ///
    static std::string const PATH_DELIMITER;
};


} // parsing
} // utility


namespace utility {
namespace convertion {
namespace implementation {


template<>
std::string convert(utility::parsing::INode::TSharedPtr const &node);


} // implementation
} // convertion
} // utility


#endif // HEADER_NODE_BA28CA42_211E_44C4_BA4F_4CF3BD0B9B34
