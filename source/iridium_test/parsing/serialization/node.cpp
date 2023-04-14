#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/parsing/implementation/parser_xml.h>
#include <iridium/parsing/implementation/parser_json.h>
#include <iridium/io/fs/files.h>
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

    DEFINE_NODE_EXTERNAL(ExternalRoot)
DEFINE_ROOT_NODE_END()


DEFINE_ROOT_NODE_BEGIN(Root2)
    DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_BEGIN(SubItem1)
        DEFINE_ATTRIBUTE(C::TEnum, Enum, C::TEnum::ENUM1)
    DEFINE_NODE_LIST_END(SubItem1)
    DEFINE_ATTRIBUTE_LIST(string, List)
DEFINE_ROOT_NODE_END()


TEST(serialization) {
//    int \#value;

    {
        auto root_node = CNode::create("root2");
        root_node->addChild("sub-item1")->addChild("enum", "ENUM2");

        TRoot2 root2(root_node);

//        ASSERT(1 , equal, root2.getNode()->findChilds("/sub-item1").size());
        root2.SubItem1.begin()->Enum.get(); // assert no throw

//        root2.List.add("item1");
//        root2.List.add("item2");
//        ASSERT(2 , equal, root2.getNode()->findChilds("/list").size());
    }

    ASSERT("camel5-struct-name" , equal, convertCamelToSplittedBySymbol("Camel5StructName", '-'));
    ASSERT(""                   , equal, convertCamelToSplittedBySymbol("", '-'));
    ASSERT("a"                  , equal, convertCamelToSplittedBySymbol("A", '-'));

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

    ASSERT( static_cast<bool>(root.ThirdItem.ThirdItem_ptr.get()));
    ASSERT(!static_cast<bool>(root.ThirdItem.ThirdItem_ptr.get()->ThirdItem_ptr.get()));
    ASSERT("5"  , equal, root.ThirdItem.Value.get())
    ASSERT("55" , equal, root.ThirdItem.ThirdItem_ptr.get()->Value.get())

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

    ASSERT("5432112345"     , equal, result);
    ASSERT("defaultValue1"  , equal, root.ExternalRoot.AttributeOne.get());
    ASSERT("value11"        , equal, root.ExternalRoot.FirstItem.AttributeOne.get());
    ASSERT(55               , equal, root.ExternalRoot.FirstItem.AttributeTwo.get());
    ASSERT(C::TEnum::ENUM1  , equal, root.ExternalRoot.FirstItem.Enum.get());
    
    ASSERT((TRoot(nullptr)), std::exception);
    {
        INode::TSharedPtr node = CNode::create("root_wrong");
        ASSERT((TRoot(node)), std::exception);
        node->addChild("first-item")->addChild("attribute-two", "55");
        ASSERT((TRoot(node)), std::exception);
    }
    
    {
        INode::TSharedPtr node = CNode::create("root");
        ASSERT((TRoot(node)), std::exception);
        node->addChild("first-item")->addChild("attribute-two", "55");
        ASSERT((TRoot(node)), std::exception);
    }
}


//namespace benchmark {
//
//
//DEFINE_ROOT_NODE_BEGIN(Root, '_')
//    DEFINE_NODE_LIST_BEGIN(Array)
//        DEFINE_ATTRIBUTE(uint64_t, Id)
//        DEFINE_ATTRIBUTE(string, Type)
//        DEFINE_NODE_BEGIN(Actor)
//            DEFINE_ATTRIBUTE(uint64_t, Id)
//            DEFINE_ATTRIBUTE(string, Login)
//            DEFINE_ATTRIBUTE(string, Url)
//            DEFINE_ATTRIBUTE(string, AvatarUrl)
//        DEFINE_NODE_END(Actor)
//        DEFINE_NODE_BEGIN(Repo)
//            DEFINE_ATTRIBUTE(uint64_t, Id)
//            DEFINE_ATTRIBUTE(string, Name)
//            DEFINE_ATTRIBUTE(string, Url)
//        DEFINE_NODE_END(Repo)
//        DEFINE_NODE_BEGIN(Payload)
//            DEFINE_ATTRIBUTE(uint64_t, PushId, 0)
//            DEFINE_ATTRIBUTE(uint64_t, Size, 0)
//            DEFINE_ATTRIBUTE(uint64_t, DistinctSize, 0)
//            DEFINE_ATTRIBUTE(string, Ref, "")
//            DEFINE_ATTRIBUTE(string, RefType, "")
//            DEFINE_ATTRIBUTE(string, MasterBranch, "")
//            DEFINE_ATTRIBUTE(string, PusherType, "")
//            DEFINE_NODE_LIST_BEGIN(Commits)
//                DEFINE_ATTRIBUTE(string, Sha)
//                DEFINE_NODE_BEGIN(Author)
//                    DEFINE_ATTRIBUTE(string, Name, "")
//                    DEFINE_ATTRIBUTE(string, Email)
//                DEFINE_NODE_END(Author)
//            DEFINE_NODE_LIST_END(Commits)
//        DEFINE_NODE_END(Payload)
//        DEFINE_ATTRIBUTE(std::chrono::system_clock::time_point, CreatedAt)
//    DEFINE_NODE_LIST_END(Array)
//DEFINE_ROOT_NODE_END();
//
//
//}
//
//
//// apple m1, input json 42 MB
//// C++
//// reading   time is 26333 microseconds
//// parsing   time is 236776 microseconds
//// composing time is 245826 microseconds
//// serialize time is 63970 microseconds
//// total serizliztion time is 300746 microseconds
//// Swift 5
//// 0.769556 seconds = 769556 microseconds
//// (~x2,5 slower)
//TEST(benchmark) {
//    using implementation::CJSONParser;
//    using iridium::io::fs::readFile;
//    using iridium::io::fs::writeFile;
//
//    auto now    = std::chrono::system_clock::now();
//    auto parser = CJSONParser::create();
//    auto file   = readFile("sample.json");
//    LOGT << "reading   time is " << std::chrono::system_clock::now() - now;
//
//    now = std::chrono::system_clock::now();
//    auto node = parser->parse(file);
//    std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - now);
//    LOGT << "parsing   time is " << std::chrono::system_clock::now() - now;
//
//    now = std::chrono::system_clock::now();
//    auto json_string = parser->compose(node);
//    LOGT << "composing time is " << std::chrono::system_clock::now() - now;
//
//    now = std::chrono::system_clock::now();
//    benchmark::TRoot json_serialized(node);
//    LOGT << "serialize time is " << std::chrono::system_clock::now() - now;
//    LOGT << json_serialized.Array.size();
//    writeFile("sample.composed.json", json_string);
//
//}


} // serialization
} // parsing
} // iridium


IMPLEMENT_ENUM(iridium::parsing::serialization::C::TEnum)
