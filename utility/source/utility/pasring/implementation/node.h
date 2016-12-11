#ifndef HEADER_NODE_256EA202_6B2A_4189_8424_A79E92A96504
#define HEADER_NODE_256EA202_6B2A_4189_8424_A79E92A96504


#include <string>
#include <list>

#include "utility/pasring/node.h"
#include "utility/smart_ptr.h"


namespace utility {
namespace parsing {
namespace implementation {


class CNode: public INode {
public:
    DEFINE_CREATE(CNode)
    ///
    explicit CNode(std::string const &name, std::string const &value);
    ///
    explicit CNode(std::string const &name);
    ///
    ~CNode() = default;
    ///
    std::string getName() const override;
    ///
    std::string getValue() const override;
    ///
    void setValue(std::string const &value) override;
    ///
    INode::TNodes findChilds(std::string const &path) override;
    ///
    INode::iterator begin() override;
    ///
    INode::iterator end() override;
    ///
    INode::const_iterator begin() const override;
    ///
    INode::const_iterator end() const override;
    ///
    size_t size() const;
    ///
    bool hasChilds() const override;
    ///
    INode::TSharedPtr addChild(INode::TSharedPtr const &child_node) override;
    ///
    INode::TSharedPtr addChild(std::string const &name) override;
    ///
    INode::TSharedPtr addChild(std::string const &name, std::string const &value) override;
    ///
    INode::TSharedPtr clone() const override;
private:
    ///
    std::string m_name;
    ///
    std::string m_value;
    ///
    std::list<INode::TSharedPtr> m_nodes;
};


} // implementation
} // parsing
} // utility


#endif // HEADER_NODE_256EA202_6B2A_4189_8424_A79E92A96504
