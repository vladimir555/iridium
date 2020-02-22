/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/parsing/implementation/parser_xml.h>
#include <iridium/parsing/implementation/parser_json.h>
#include <iridium/enum.h>

#include <iostream>


using std::string;
using std::cout;
using std::endl;

using iridium::convertion::convert;
using iridium::parsing::implementation::CNode;


namespace iridium {
namespace parsing {
namespace serialization {


struct C {
    DEFINE_ENUM(TEnum, ENUM1, ENUM2)
};


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

    DEFINE_NODE_LIST_BEGIN(Item1)
        DEFINE_NODE_BEGIN(SubItem)
            DEFINE_ATTRIBUTE(string, AttributeOne, "attribute-one")
            DEFINE_ATTRIBUTE(int, AttributeTwo, 555)
        DEFINE_NODE_END(SubItem)
    DEFINE_NODE_LIST_END(Item1)

    DEFINE_ATTRIBUTE_LIST(std::string, Array)
DEFINE_ROOT_NODE_END()


DEFINE_ROOT_NODE_BEGIN(Root2)
    DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_BEGIN(SubItem1)
        DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_END(SubItem1)
    DEFINE_ATTRIBUTE_LIST(string, List)
DEFINE_ROOT_NODE_END()


TEST(serialization) {
    {
        auto root_node = CNode::create("root2");
        root_node->addChild("sub-item1")->addChild("enum", "ENUM2");

        TRoot2 root2(root_node);

        ASSERT(1 , equal, root2.getNode()->findChilds("/sub-item1").size());
        root2.SubItem1.begin()->Enum.get(); // assert no throw

        root2.List.add("item1");
        root2.List.add("item2");
        ASSERT(2 , equal, root2.getNode()->findChilds("/list").size());

        return;
    }


    ASSERT("camel5-struct-name", equal, convertCamelToDashed("Camel5StructName"));
    ASSERT("" , equal, convertCamelToDashed(""));
    ASSERT("a", equal, convertCamelToDashed("A"));

    INode::TSharedPtr node = CNode::create("root");

    {
        auto item = node->addChild("first-item");
        item->addChild("attribute-two", "55");
        item->addChild("enum", "ENUM2");
    }

    node->addChild("second-item")->addChild("attribute-one", "second-item-value");

    TRoot(node).getNode(); // assert no throw

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

    ASSERT("defaultValue1", equal, static_cast<string>   (root.FirstItem.AttributeOne));
    ASSERT(55             , equal, static_cast<int>      (root.FirstItem.AttributeTwo));
    ASSERT(C::TEnum::ENUM2, equal,                        root.FirstItem.Enum.get());
    ASSERT("defaultValue1", equal, root.getNode()->getChild("first-item")->getChild("attribute-one")->getValue());

    ASSERT(2, equal, root.Item1.size());
    auto i = root.Item1.begin();
    i++++;
    ASSERT(555, equal, i->SubItem.AttributeTwo.get());

    TRoot::TItem1 item;
    item.SubItem.AttributeTwo = 55555;
    root.Item1.add(item);

    ASSERT(3, equal, root.Item1.size());
    i++;
    ASSERT(55555, equal, i->SubItem.AttributeTwo.get());

    string result;
    for (auto const &i: root.Array)
        result += i.get();
    ASSERT("54321", equal, result);

    root.Array.add(string("12345"));

    result.clear();
    for (auto const &i: root.Array)
        result += i.get();
    ASSERT("5432112345", equal, result);
}


} // serialization
} // parsing
} // iridium


IMPLEMENT_ENUM(iridium::parsing::serialization::C::TEnum)
