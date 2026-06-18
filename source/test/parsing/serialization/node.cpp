#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/parsing/implementation/xml_parser.h>
#include <iridium/parsing/implementation/json_parser.h>
#include <iridium/io/fs/files.h>
#include <iridium/enum.h>
#include <iridium/assert.h>


using std::string;

using iridium::convertion::convert;
using iridium::parsing::implementation::CNode;
using iridium::assertExists;


namespace iridium::parsing::serialization {


struct C {
    DEFINE_ENUM(TEnum, ENUM1, ENUM2)
};


DEFINE_ROOT_NODE_BEGIN(ExternalRoot)
    DEFINE_ATTRIBUTE(string, AttributeOne, "defaultValue1")
    DEFINE_NODE_BEGIN(FirstItem)
        DEFINE_ATTRIBUTE(string, AttributeOne, "defaultValue1")
        DEFINE_ATTRIBUTE(int, AttributeTwo)
        DEFINE_ATTRIBUTE(C::TEnum, Enum)
    DEFINE_NODE_END(FirstItem)
DEFINE_ROOT_NODE_END()


DEFINE_ROOT_NODE_BEGIN(Root)
    DEFINE_ATTRIBUTE(string, AttributeOne, "defaultValue1")
    DEFINE_NODE_BEGIN(FirstItem)
        DEFINE_ATTRIBUTE(string, AttributeOne, "defaultValue1")
        DEFINE_ATTRIBUTE(int, AttributeTwo)
        DEFINE_ATTRIBUTE(C::TEnum, Enum)
    DEFINE_NODE_END(FirstItem)

    DEFINE_NODE_BEGIN(SecondItem)
        DEFINE_ATTRIBUTE(string, AttributeOne, "defaultValue2")
    DEFINE_NODE_END(SecondItem)

    // recursive nodes
    DEFINE_NODE_BEGIN(ThirdItem)
        DEFINE_ATTRIBUTE(string, Value)
        DEFINE_NODE_PTR(ThirdItem)
    DEFINE_NODE_END(ThirdItem)

    DEFINE_NODE_LIST_BEGIN(Item1)
        DEFINE_NODE_BEGIN(SubItem)
            DEFINE_ATTRIBUTE(string, AttributeOne, "attribute-one")
            DEFINE_ATTRIBUTE(int, AttributeTwo, 555)
        DEFINE_NODE_END(SubItem)
    DEFINE_NODE_LIST_END(Item1)

    DEFINE_ATTRIBUTE_LIST(std::string, Array)

    DEFINE_NODE_BEGIN(TextNode)
        DEFINE_ATTRIBUTE(string, TextNodeAttribute, "text-node-attribute-value")
//        DEFINE_ATTRIBUTE(string, #Text, "text")
    DEFINE_NODE_END(TextNode)

    // including external node
    DEFINE_NODE_EXTERNAL(ExternalRoot)

DEFINE_ROOT_NODE_END()


DEFINE_ROOT_NODE_BEGIN(Root2)
    DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_BEGIN(SubItem1)
        DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_END(SubItem1)
    DEFINE_ATTRIBUTE_LIST(string, List)
DEFINE_ROOT_NODE_END()


TEST(node_to_struct) {
//    {
//        INode::TSharedPtr node = CNode::create("root");
//        TRoot root(node);
//        LOGT << node;
//        root.Array.add("12345");
//        LOGT << node;
//    }

    {
        INode::TSharedPtr root_node = CNode::create("root2");
        root_node->addChild("sub-item1")->addChild("enum", "ENUM2");

        TRoot2 root2(root_node);

//        ASSERT(1 , equal, root2.getNode()->findChilds("/sub-item1").size());
        root2.SubItem1.begin()->Enum.get(); // assert no throw

//        root2.List.add("item1");
//        root2.List.add("item2");
//        ASSERT(2 , equal, root2.getNode()->findChilds("/list").size());
    }

    using iridium::parsing::serialization::TNamingStrategyCPPToNode;
    ASSERT("camel5-struct-name" , equal, convertNameCPPToNode("Camel5StructName", TNamingStrategyCPPToNode::CAMEL_CASE_TO_LOWER_KEBAB));
    ASSERT(""                   , equal, convertNameCPPToNode("",  TNamingStrategyCPPToNode::CAMEL_CASE_TO_LOWER_KEBAB));
    ASSERT("a"                  , equal, convertNameCPPToNode("A", TNamingStrategyCPPToNode::CAMEL_CASE_TO_LOWER_KEBAB));

    INode::TSharedPtr node = CNode::create("root");

    {
        auto item = node->addChild("first-item");
        item->addChild("attribute-two", "55");
        item->addChild("enum", "ENUM2");
    }

    {
        node->addChild("second-item")->addChild("attribute-one", "second-item-value");
        auto i = node->addChild("third-item");
        i->addChild("value", "5");
        i->addChild("third-item")->addChild("value", "55");
    }

    {
        auto item = node->addChild("external-root")->addChild("first-item");
        item->addChild("attribute-one", "value11");
        item->addChild("attribute-two", "55");
        item->addChild("enum", "ENUM1");
    }

    TRoot(node->clone()).getNode(); // assert no throw

    {
        auto item = node->addChild("item1")->addChild("sub-item");
        item->addChild("attribute-one", "value1");
        item->addChild("attribute-two", "5");
    }

    {
        auto item = node->addChild("item1")->addChild("sub-item");
        item->addChild("attribute-one", "Value2");
    }

    {
        node->addChild("array", "5");
        node->addChild("array", "4");
        node->addChild("array", "3");
        node->addChild("array", "2");
        node->addChild("array", "1");
    }

    TRoot root(node);

    ASSERT( static_cast<bool>(root.ThirdItem.ThirdItem_ptr.get()));
    ASSERT(!static_cast<bool>(root.ThirdItem.ThirdItem_ptr.get()->ThirdItem_ptr.get()));
    ASSERT("5"  , equal, root.ThirdItem.Value.get())
    ASSERT("55" , equal, root.ThirdItem.ThirdItem_ptr.get()->Value.get())

    ASSERT("defaultValue1", equal, static_cast<string>   (root.FirstItem.AttributeOne));
    ASSERT(55             , equal, static_cast<int>      (root.FirstItem.AttributeTwo));
    ASSERT(C::TEnum::ENUM2, equal,                        root.FirstItem.Enum.get());
    ASSERT("defaultValue1", equal, root.getNode()->getChild("first-item")->getChild("attribute-one")->getValue());

    ASSERT(root.FirstItem.AttributeOne.isDefault())

    {
//        LOGT << root.getNode();
        root.FirstItem.AttributeOne.set("defaultValue11");
//        LOGT << root.getNode();
        ASSERT("defaultValue11", equal, static_cast<string>(root.FirstItem.AttributeOne));

        string value = assertComplete(
            root.getNode()->slice("/first-item/attribute-one"),
            "expected value does not exists").front()->getValue();
        ASSERT("defaultValue11", equal, value);
        ASSERT(!root.FirstItem.AttributeOne.isDefault())
    }

    ASSERT(2, equal, root.Item1.size());
    auto i = root.Item1.begin();
    std::advance(i, 1);
    ASSERT(555, equal, i->SubItem.AttributeTwo.get());

    TRoot::TItem1 item;
    item.SubItem.AttributeTwo = 55555;
    root.Item1.add(item);

    ASSERT(3, equal, root.Item1.size());
    std::advance(i, 1);
    ASSERT(55555, equal, i->SubItem.AttributeTwo.get());

    string result;
    for (auto const &i_: root.Array)
        result += i_.get();
    ASSERT("54321", equal, result);

//    LOGT << node;
    root.Array.add(string("12345"));
//    LOGT << node;


    result.clear();
    for (auto const &i_: root.Array)
        result += i_.get();

    ASSERT("5432112345"     , equal, result);
    ASSERT("defaultValue1"  , equal, root.ExternalRoot.AttributeOne.get());
    ASSERT("value11"        , equal, root.ExternalRoot.FirstItem.AttributeOne.get());
    ASSERT(55               , equal, root.ExternalRoot.FirstItem.AttributeTwo.get());
    ASSERT(C::TEnum::ENUM1  , equal, root.ExternalRoot.FirstItem.Enum.get());

    ASSERT((TRoot(nullptr)), std::exception);
    {
        INode::TSharedPtr node_ = CNode::create("root_wrong");
        ASSERT((TRoot(node_)), std::exception);
        node_->addChild("first-item")->addChild("attribute-two", "55");
        ASSERT((TRoot(node_)), std::exception);
    }

    {
        INode::TSharedPtr node_ = CNode::create("root");
        ASSERT((TRoot(node_)), std::exception);
        node_->addChild("first-item")->addChild("attribute-two", "55");
        ASSERT((TRoot(node_)), std::exception);
    }
}


} // iridium::parsing::serialization


// IMPLEMENT_ENUM(iridium::parsing::serialization::C::TEnum)
