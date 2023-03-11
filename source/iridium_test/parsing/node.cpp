/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>
#include <iostream>
#include <iridium/parsing/node.h>
#include <iridium/parsing/implementation/node.h>


using iridium::convertion::convert;
using std::string;
using std::cout;
using std::endl;
using iridium::parsing::implementation::CNode;


namespace iridium {
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


//string const expected_xml = "";


TEST(slice) {
    auto node = createTestNode();

    ASSERT(node->slice("/wrong_node/").empty());
    ASSERT(node->slice("wrong_node/").empty());
    ASSERT(node->slice("").empty());

    ASSERT(3, equal, node->slice("/").size());
    ASSERT(3, equal, node->slice("/item").size());
}


} // parsing
} // iridium
