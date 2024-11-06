#include <iridium/testing/tester.h>
#include <iridium/parsing/node.h>
#include <iridium/parsing/implementation/node.h>


using iridium::convertion::convert;
using std::string;
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


TEST(slice) {
    auto node = createTestNode();

    ASSERT(node->slice("/wrong_node/").empty());
    ASSERT(node->slice("wrong_node/").empty());
    ASSERT(node->slice("").empty());

    ASSERT(3, equal, node->slice("/").size());
    ASSERT(3, equal, node->slice("/item").size());
}


// AI generated


TEST(get_name) {
    CNode node("test_node", "test_value");
    ASSERT(node.getName() == "test_node");
}


TEST(get_value) {
    CNode node("test_node", "test_value");
    ASSERT(node.getValue() == "test_value");
}


TEST(set_value) {
    CNode node("test_node", "test_value");
    node.setValue("new_value");
    ASSERT(node.getValue() == "new_value");
}


TEST(add_child) {
    CNode node("parent_node");
    auto child = node.addChild("child_node", "child_value");
    ASSERT(child->getName() == "child_node");
    ASSERT(child->getValue() == "child_value");
    ASSERT(node.size() == 1);
}


TEST(has_childs) {
    CNode node("parent_node");
    ASSERT(!node.hasChilds());
    node.addChild("child_node", "child_value");
    ASSERT(node.hasChilds());
}


TEST(get_child) {
    CNode node("parent_node");
    node.addChild("child_node", "child_value");
    auto child = node.getChild("child_node");
    ASSERT(child != nullptr);
    ASSERT(child->getName() == "child_node");
    ASSERT(child->getValue() == "child_value");
}


TEST(clone) {
    CNode node("root_node", "root_value");
    node.addChild("child1", "value1");
    node.addChild("child2", "value2");

    auto clonedNode = node.clone();
    ASSERT(clonedNode->getName() == "root_node");
    ASSERT(clonedNode->getValue() == "root_value");
    ASSERT(clonedNode->size() == 2);

    auto child1 = clonedNode->getChild("child1");
    ASSERT(child1 != nullptr);
    ASSERT(child1->getValue() == "value1");

    auto child2 = clonedNode->getChild("child2");
    ASSERT(child2 != nullptr);
    ASSERT(child2->getValue() == "value2");
}


TEST(slice_invalid_path) {
    CNode node("root_node");
    node.addChild("child1");
    node.addChild("child2");

    auto slicedNodes = node.slice("/invalid_path");
    ASSERT(slicedNodes.size() == 0);
}


TEST(get_child_nonexistent) {
    CNode node("parent_node");
    auto child = node.getChild("nonexistent_child");
    ASSERT(child == nullptr);
}


TEST(add_child_nullptr) {
    CNode node("parent_node");
    auto result = node.addChild(static_cast<INode::TSharedPtr>(nullptr));
    ASSERT(result == nullptr);
    ASSERT(node.size() == 0);
}


TEST(add_child_empty_name) {
    CNode node("parent_node");
    auto child = node.addChild("", "value");
    ASSERT(child->getName().empty());
    ASSERT(node.size() == 1);
}


TEST(set_value_empty_string) {
    CNode node("test_node", "test_value");
    node.setValue("");
    ASSERT(node.getValue().empty());
}


TEST(clone_with_empty_children) {
    CNode node("root_node", "root_value");
    auto clonedNode = node.clone();
    ASSERT(clonedNode->getName() == "root_node");
    ASSERT(clonedNode->getValue() == "root_value");
    ASSERT(clonedNode->size() == 0);
}


TEST(slice_empty_path) {
    CNode node("root_node");
    node.addChild("child1");
    auto slicedNodes = node.slice("");
    ASSERT(slicedNodes.size() == 0);
}


TEST(slice_root_path) {
    CNode node("root_node");
    auto slicedNodes = node.slice("/");
    ASSERT(slicedNodes.size() == 0);
}


TEST(get_child_empty_name) {
    CNode node("parent_node");
    auto child = node.getChild("");
    ASSERT(child == nullptr);
}


} // parsing
} // iridium
