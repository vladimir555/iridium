#include <gtest/gtest.h>

#include <utility/parsing/serialization/node.h>
#include <utility/parsing/implementation/node.h>
#include <utility/parsing/implementation/parser_xml.h>
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
            DEFINE_ATTRIBUTE_DEFAULT(int, AttributeTwo, 555)
        DEFINE_NODE_END(SubItem)
    DEFINE_NODE_LIST_END(Item1)

    DEFINE_ATTRIBUTE_LIST(std::string, Array)
DEFINE_ROOT_NODE_END()


TEST(parsing, serialization) {

    ASSERT_EQ("camel5-struct-name", convertCamelToDashed("Camel5StructName"));
    ASSERT_EQ("",  convertCamelToDashed(""));
    ASSERT_EQ("a", convertCamelToDashed("A"));

    INode::TSharedPtr node = CNode::create("root");

    {
        auto item = node->addChild("first-item");
        item->addChild("attribute-two", "55");
        item->addChild("enum", "ENUM2");
    }

    node->addChild("second-item")->addChild("attribute-one", "second-item-value");

    ASSERT_THROW(TRoot(node), std::exception);

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

    ASSERT_EQ("defaultValue1", static_cast<string>   (root.FirstItem.AttributeOne));
    ASSERT_EQ(55             , static_cast<int>      (root.FirstItem.AttributeTwo));
    ASSERT_EQ(C::TEnum::ENUM2,                        root.FirstItem.Enum.get());
    ASSERT_EQ("defaultValue1", root.getNode()->getChild("first-item")->getChild("attribute-one")->getValue());

    ASSERT_EQ(2, root.Item1.size());
    auto i = root.Item1.begin();
    i++++;
    ASSERT_EQ(555, i->SubItem.AttributeTwo.get());

    TRoot::TItem1 item;
    item.SubItem.AttributeTwo = 55555;
    root.Item1.add(item);

    ASSERT_EQ(3, root.Item1.size());
    i++;
    ASSERT_EQ(55555, i->SubItem.AttributeTwo.get());

    string result;
    for (auto const &i: root.Array)
        result += i.get();
    ASSERT_EQ("54321", result);

    root.Array.add(string("12345"));

    result.clear();
    for (auto const &i: root.Array)
        result += i.get();
    ASSERT_EQ("5432112345", result);
}


} // serialization
} // parsing
} // utility


IMPLEMENT_ENUM(utility::parsing::serialization::C::TEnum)
