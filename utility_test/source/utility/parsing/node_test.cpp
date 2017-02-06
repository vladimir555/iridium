#include <gtest/gtest.h>
#include <iostream>
#include <utility/pasring/node.h>
#include <utility/pasring/implementation/node.h>


using utility::convertion::convert;
using std::string;
using std::cout;
using std::endl;
using utility::parsing::implementation::CNode;


namespace utility {
namespace parsing {


INode::TSharedPtr createTestNode() {
    auto root_node = CNode::create("root");

    {
        auto item = root_node->addChild("item");
        item->addChild("attr_value", "value_1");
        item->addChild("attr_name", "name_1");
    }

    {
        auto item = root_node->addChild("item");
        item->addChild("attr_value", "value_2");
        item->addChild("attr_name", "name_2");
        auto sub_item = item->addChild("sub_item");
        sub_item->addChild("item", "sub_item_attr_value_5");
    }

    {
        auto item = root_node->addChild("item");
        item->addChild("", "text");
    }

    return root_node;
}


string const expected_xml = "";


TEST(parsing, nodes) {
    auto node = createTestNode();

    ASSERT_TRUE(node->findChilds("/wrong_node/").empty());
    ASSERT_TRUE(node->findChilds("wrong_node/").empty());
    ASSERT_TRUE(node->findChilds("/").empty());
    ASSERT_TRUE(node->findChilds("").empty());
    ASSERT_EQ(3, node->findChilds("/item").size());
}


} // parsing
} // utility
