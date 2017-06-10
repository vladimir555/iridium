#include <gtest/gtest.h>

#include <utility/parsing/serialization/node.h>
#include <utility/parsing/implementation/node.h>
#include <utility/enum.h>

#include <iostream>


using std::string;
using std::cout;
using std::endl;

using utility::convertion::convert;
using utility::parsing::implementation::CNode;


namespace utility {
namespace parsing {
namespace serialization {


struct C {
    DEFINE_ENUM(TEnum, ENUM1, ENUM2)
};


DEFINE_ROOT_NODE_BEGIN(Root)

    DEFINE_NODE_BEGIN(FirstItem)
        DEFINE_ATTRIBUTE_DEFAULT(string, AttributeOne, "defaultValue1")
        DEFINE_ATTRIBUTE(int, AttributeTwo)
        DEFINE_ATTRIBUTE(C::TEnum, Enum)
    DEFINE_NODE_END(FirstItem)

    DEFINE_NODE_BEGIN(SecondItem)
        DEFINE_ATTRIBUTE_DEFAULT(string, AttributeOne, "defaultValue2")
    DEFINE_NODE_END(SecondItem)

    DEFINE_NODE_LIST_BEGIN(Item1)
        DEFINE_NODE_BEGIN(SubItem)
            DEFINE_ATTRIBUTE_DEFAULT(string, AttributeOne, "attribute-one")
            DEFINE_ATTRIBUTE_DEFAULT(int, AttributeTwo, 5)
        DEFINE_NODE_END(SubItem)
    DEFINE_NODE_LIST_END(Item1)

DEFINE_ROOT_NODE_END()


TEST(parsing, serialization) {
    INode::TSharedPtr node = CNode::create("root");

    {
        auto item = node->addChild("first-item");
        item->addChild("attribute-two", "55");
        item->addChild("enum", "ENUM2");
    }

    node->addChild("second_item")->addChild("attribute-one", "second-item-value");

    {
        auto item = node->addChild("item-1")->addChild("sub-item");
        item->addChild("attribute-one", "value1");
        item->addChild("attribute-two", "5");
    }

    {
        auto item = node->addChild("item-1")->addChild("sub-item");
        item->addChild("attribute-one", "Value2");
        item->addChild("attribute-two", "5");
    }

    ASSERT_EQ("camel5-struct-name", convertCamelToDashed("Camel5StructName"));
    ASSERT_EQ("",  convertCamelToDashed(""));
    ASSERT_EQ("a", convertCamelToDashed("A"));

    TRoot root(node);

    ASSERT_EQ("defaultValue1", static_cast<string>   (root.FirstItem.AttributeOne));
    ASSERT_EQ(55             , static_cast<int>      (root.FirstItem.AttributeTwo));
    ASSERT_EQ(C::TEnum::ENUM2, root.FirstItem.Enum.get());
}


} // serialization
} // parsing
} // utility


IMPLEMENT_ENUM(utility::parsing::serialization::C::TEnum)
