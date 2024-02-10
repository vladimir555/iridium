#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/parsing/implementation/parser_xml.h>
#include <iridium/parsing/implementation/parser_json.h>
#include <iridium/io/fs/files.h>
#include <iridium/enum.h>
#include <iridium/assert.h>


using std::string;

using iridium::convertion::convert;
using iridium::parsing::implementation::CNode;
using iridium::assertExists;


//#include <simdjson.h>
//#include <nlohmann/json.hpp>
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


TEST(serialization) {
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
    for (auto const &i: root.Array)
        result += i.get();
    ASSERT("54321", equal, result);

//    LOGT << node;
    root.Array.add(string("12345"));
//    LOGT << node;


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
////struct S1 {
////    struct S2 {
////        S2(S1 const * const s1) {};
////    } s2 = this;
////
////} s1;
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
////        DEFINE_ATTRIBUTE(std::chrono::system_clock::time_point, CreatedAt)
//    DEFINE_NODE_LIST_END(Array)
//DEFINE_ROOT_NODE_END();
//
//
//} // benchmark


// apple m1, input json 42 MB
// C++
// reading   time is 26333 microseconds
// parsing   time is 236776 microseconds
// composing time is 245826 microseconds
// serialize time is 63970 microseconds
// total serizliztion time is 300746 microseconds
// Swift 5
// 0.769556 seconds = 769556 microseconds
// (~x2,5 slower)
// simdjson (x5 faster)
// nlohmann (x1.11 faster)
// UPDATE: iridium json parsing + serialization time is 192965 microseconds
//TEST(benchmark_iridium) {
//    using implementation::CJSONParser;
//    using iridium::io::fs::readFile;
//    using iridium::io::fs::writeFile;
//
//    auto now        = std::chrono::system_clock::now();
//    auto parser     = CJSONParser::create();
//    auto json_text  = readFile("sample.json");
//    LOGI << "reading   time is " << std::chrono::system_clock::now() - now;
//
//    now = std::chrono::system_clock::now();
//    auto node = parser->parse(json_text);
//    benchmark::TRoot json_serialized(node);
//    LOGI << "parsing   time is " << std::chrono::system_clock::now() - now;
//    LOGI << "array size: " << json_serialized.Array.size();
//
////    now = std::chrono::system_clock::now();
////    auto json_string = parser->compose(node);
////    LOGI << "composing time is " << std::chrono::system_clock::now() - now;
//
////    now = std::chrono::system_clock::now();
////    benchmark::TRoot json_serialized(node);
////    LOGI << "serialize time is " << std::chrono::system_clock::now() - now;
//
////    writeFile("sample.composed.json", json_string);
//}


//namespace benchmark {
//
//
//struct TJsonRoot {
//    struct TItem {
//        uint64_t    Id;
//        string      Type;
//
//        struct TActor {
//            uint64_t    Id;
//            string      Login;
//            string      Url;
//            string      AvatarUrl;
//        };
//        TActor Actor;
//
//        struct TRepo {
//            uint64_t    Id;
//            string      Name;
//            string      Url;
//        };
//        TRepo Repo;
//
//        struct TPayload {
//            uint64_t    PushId;
//            uint64_t    Size;
//            uint64_t    DistinctSize;
//            string      Ref;
//            string      RefType;
//            string      MasterBranch;
//            string      PusherType;
//
//            struct TCommit {
//                string Sha;
//
//                struct TAuthor {
//                    string Name;
//                    string Email;
//                };
//                TAuthor Author;
//            };
//            std::list<TCommit> Commits;
//        };
//        TPayload    Payload;
//        std::chrono::system_clock::time_point
//                    CreatedAt;
//    };
//    std::list<TItem> Array;
//};
//
//
//} // benchmark



// todo: simdjson library memory leak on macos M1 arm !
//TEST(benchmark_simdjson) {
//    auto now = std::chrono::system_clock::now();
//
//    simdjson::ondemand::parser      parser;
//    simdjson::padded_string         json_text   = simdjson::padded_string::load("sample.json");
////    string file = iridium::io::fs::readFile("sample.json");
////    string  file = "[{\"id\": 2489651045}, {\"id\": 2}]";
//    benchmark::TJsonRoot            root        = { };
//
//    LOGI << "reading   time is " << std::chrono::system_clock::now() - now;
//
//    simdjson::ondemand::document    document    = parser.iterate(json_text);
//
//    now = std::chrono::system_clock::now();
//
//    for (auto json_item: document) {
//        benchmark::TJsonRoot::TItem item {
//            .Id     = json_item["id"].get_uint64_in_string().value(),
//            .Type   = json_item["type"].get<std::string_view>().value().data(),
//            .Actor  = benchmark::TJsonRoot::TItem::TActor {
//                .Id         = json_item["actor"]["id"].get<uint64_t>().value(),
//                .Login      = json_item["actor"]["login"].get<std::string_view>().value().data(),
//                .Url        = json_item["actor"]["url"].get<std::string_view>().value().data(),
//                .AvatarUrl  = json_item["actor"]["avatar_url"].get<std::string_view>().value().data(),
//            },
//            .Repo   = benchmark::TJsonRoot::TItem::TRepo {
//                .Id         = json_item["repo"]["id"].get<uint64_t>().value(),
//                .Name       = json_item["repo"]["name"].get<std::string_view>().value().data(),
//                .Url        = json_item["repo"]["url"].get<std::string_view>().value().data(),
//            },
//        };
//
//        json_item["payload"].find_field("push_id").get(item.Payload.PushId);
//        json_item["payload"].find_field("size").get(item.Payload.Size);
//        json_item["payload"].find_field("distinct_size").get(item.Payload.DistinctSize);
//
//        {
//            {
//                std::string_view sv;
//                json_item["payload"].find_field("ref").get(sv);
//                item.Payload.Ref = sv;
//            }
//            {
//                std::string_view sv;
//                json_item["payload"].find_field("ref_type").get(sv);
//                item.Payload.RefType = sv;
//            }
//            {
//                std::string_view sv;
//                json_item["payload"].find_field("master_branch").get(sv);
//                item.Payload.MasterBranch = sv;
//            }
//            {
//                std::string_view sv;
//                json_item["payload"].find_field("pusher_type").get(sv);
//                item.Payload.PusherType = sv;
//            }
//
//            benchmark::TJsonRoot::TItem::TPayload::TCommit commit;
//            for (auto json_commit: json_item["payload"]["commits"]) {
//                {
//                    std::string_view sv;
//                    json_commit.find_field("sha").get(sv);
//                    commit.Sha = sv;
//                }
//
//                {
//                    std::string_view sv;
//                    json_commit["author"].find_field("name").get(sv);
//                    commit.Author.Name = sv;
//                }
//                {
//                    std::string_view sv;
//                    json_commit["author"].find_field("email").get(sv);
//                    commit.Author.Email = sv;
//                }
//            }
//            item.Payload.Commits.push_back(commit);
//        }
//
//        root.Array.push_back(item);
//    }
//
//    LOGI << "parsing   time is " << std::chrono::system_clock::now() - now;
//    LOGI << "array size: " << root.Array.size();
//}


//TEST(benchmark_nlohmann) {
//    auto now        = std::chrono::system_clock::now();
//    auto json_text  = iridium::io::fs::readFile("sample.json");
//
//    benchmark::TJsonRoot root = { };
//
//    LOGI << "reading   time is " << std::chrono::system_clock::now() - now;
//    now = std::chrono::system_clock::now();
//
//    auto document   = nlohmann::json::parse(json_text);
//
//    for (auto const &json_item: document) {
//        benchmark::TJsonRoot::TItem item {
//            .Id     = std::stoull(json_item["id"].get<std::string>()),
//            .Type   = json_item["type"].get<std::string>(),
//            .Actor  = benchmark::TJsonRoot::TItem::TActor {
//                .Id     = std::stoull(json_item["id"].get<std::string>()),
//                .Login      = json_item["actor"]["login"].get<std::string>(),
//                .Url        = json_item["actor"]["url"].get<std::string>(),
//                .AvatarUrl  = json_item["actor"]["avatar_url"].get<std::string>(),
//            },
//            .Repo       = benchmark::TJsonRoot::TItem::TRepo {
//                .Id         = json_item["repo"]["id"].get<uint64_t>(),
//                .Name       = json_item["repo"]["name"].get<std::string>(),
//                .Url        = json_item["repo"]["url"].get<std::string>(),
//            },
//            .Payload    = benchmark::TJsonRoot::TItem::TPayload {
//                .PushId         = json_item["payload"].value("push_id", uint64_t(0)),
//                .Size           = json_item["payload"].value("size", uint64_t(0)),
//                .DistinctSize   = json_item["payload"].value("distinct", uint64_t(0)),
//                .Ref            = json_item["payload"]["ref"].dump(),
//                .RefType        = json_item["payload"].value("ref_type", ""),
//                .MasterBranch   = json_item["payload"].value("master_branch", ""),
//                .PusherType     = json_item["payload"].value("pusher_type", ""),
//            }
//        };
//
//        if (json_item["payload"]["commits"].is_array()) {
//            for (auto const &json_commit: json_item["payload"]["commits"]) {
//                item.Payload.Commits.push_back(
//                    benchmark::TJsonRoot::TItem::TPayload::TCommit {
//                        .Sha    = json_commit["sha"].get<std::string>(),
//                        .Author = benchmark::TJsonRoot::TItem::TPayload::TCommit::TAuthor {
//                            .Name   = json_commit["author"]["name"].get<std::string>(),
//                            .Email  = json_commit["author"]["email"].get<std::string>(),
//                        }
//                    }
//                );
//            }
//        }
//
//        if (json_item["payload"]["commits"].is_object()) {
//            auto const &json_commit = json_item["payload"]["commits"];
//            item.Payload.Commits.push_back(
//                benchmark::TJsonRoot::TItem::TPayload::TCommit {
//                    .Sha    = json_commit["sha"].get<std::string>(),
//                    .Author = benchmark::TJsonRoot::TItem::TPayload::TCommit::TAuthor {
//                        .Name   = json_commit["author"]["name"].get<std::string>(),
//                        .Email  = json_commit["author"]["email"].get<std::string>(),
//                    }
//                }
//            );
//        }
//
//        root.Array.push_back(item);
//    }
//
//    LOGI << "parsing   time is " << std::chrono::system_clock::now() - now;
//    LOGI << "array size: " << root.Array.size();
//}


// not working !
// TEST(benchmark_thinkcell) {
//     auto    now = std::chrono::system_clock::now();
//     string  file;
//     file = "[{\"id\": 1}, {\"id\": 2}]";
//     // file = iridium::io::fs::readFile("sample.json");
//     LOGI << "reading   time is " << std::chrono::system_clock::now() - now;
//     now = std::chrono::system_clock::now();
//     auto callback_exception = [] (auto const &error) THROW(std::runtime_error) {
//         throw std::runtime_error("parsing error: " + std::string(error));
//     };
//     auto parser = tc::json::parser(file, tc::json::simple_error_handler(callback_exception));
//     auto const assignString = [&] (std::string &variable) noexcept {
//         return [&] {
//             auto value = parser.expect_string();
//             variable.assign(value.begin(), value.end());
//         };
//     };
//     auto const assignNumber = [&] (auto &variable) noexcept {
//         return [&] {
//             variable = parser.expect_number< typename std::remove_reference< decltype(variable) >::type >();
//         };
//     };
//     // string id;
//     int id = 0;
//     parser.expect_array();
//     while (parser.element()) {
//         parser.expect_element();
//         LOGT << "element";
//         // if (parser.object()) {
//         //     parser.expect_object();
//         //     while (auto const key = parser.key()) {
//         //         LOGT << std::string(key->begin(), key->end()) << ": " << std::string(key.value().begin(), key.value().end());
//         //     }
//         //     // LOGT << "object";
//         //     // tc::json::required(
//         //     //     tc::named<"id">(assignNumber(id))
//         //     // );
//         //     // LOGT << "element: id '" << id << "'";
//         // }
//     }
//     parser.expect_array_end();
//     parser.expect_end();
//     LOGI << "parsing   time is " << std::chrono::system_clock::now() - now;
//     // now = std::chrono::system_clock::now();
//     // compose ...
//     // LOGI << "composing time is " << std::chrono::system_clock::now() - now;
// //    now = std::chrono::system_clock::now();
// // serialize ...
// //    LOGI << "serialize time is " << std::chrono::system_clock::now() - now;
// //    LOGI << json_serialized.Array.size();
// //    writeFile("sample.composed.json", json_string);
// }


} // serialization
} // parsing
} // iridium


IMPLEMENT_ENUM(iridium::parsing::serialization::C::TEnum)


//LINUX GCC12 ARM
//make -j4 && ./iridium_test run --mode=raw /parsing/serialization/node.cpp/benchmark_iridium
//[ 84%] Built target iridium
//[100%] Built target iridium_test
//main thread: 281473770426400
//2024-02-01 06:41:51.466 I 281473770426400 RUN  /parsing/serialization/node.cpp/benchmark
//2024-02-01 06:41:51.482 I 281473770426400 reading   time is 16347616 nanoseconds
//2024-02-01 06:41:51.634 I 281473770426400 parsing   time is 151663430 nanoseconds
//2024-02-01 06:41:51.927 I 281473770426400 composing time is 293086975 nanoseconds
//2024-02-01 06:41:52.015 I 281473770426400 serialize time is 88248133 nanoseconds
//2024-02-01 06:41:52.015 I 281473770426400 array size: 11351
//2024-02-01 06:41:52.133 I 281473770426400 OK   /parsing/serialization/node.cpp/benchmark
//2024-02-01 06:41:52.133 I 281473770426400 RUN  /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 06:41:52.210 I 281473770426400 reading   time is 76336090 nanoseconds
//2024-02-01 06:41:52.257 I 281473770426400 parsing   time is 38370061 nanoseconds - x4 faster
//2024-02-01 06:41:52.257 I 281473770426400 array size: 11351
//2024-02-01 06:41:52.260 I 281473770426400 OK   /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 06:41:52.260 I 281473770426400
//passed: 2
//failed: 0
//total:  23
//LINUX CLANG14 ARM
//main thread: 281473111482400
//2024-02-01 06:43:27.421 I 281473111482400 RUN  /parsing/serialization/node.cpp/benchmark
//2024-02-01 06:43:27.443 I 281473111482400 reading   time is 21309527 nanoseconds
//2024-02-01 06:43:27.600 I 281473111482400 parsing   time is 157381520 nanoseconds
//2024-02-01 06:43:27.905 I 281473111482400 composing time is 304384106 nanoseconds
//2024-02-01 06:43:27.993 I 281473111482400 serialize time is 88791113 nanoseconds
//2024-02-01 06:43:27.994 I 281473111482400 array size: 11351
//2024-02-01 06:43:28.119 I 281473111482400 OK   /parsing/serialization/node.cpp/benchmark
//2024-02-01 06:43:28.119 I 281473111482400 RUN  /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 06:43:28.208 I 281473111482400 reading   time is 88501500 nanoseconds
//2024-02-01 06:43:28.253 I 281473111482400 parsing   time is 35895462 nanoseconds - x4.5 faster !!!
//2024-02-01 06:43:28.253 I 281473111482400 array size: 11351
//2024-02-01 06:43:28.256 I 281473111482400 OK   /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 06:43:28.256 I 281473111482400
//passed: 2
//failed: 0
//total:  23
//MACOS CLANG15 ARM
//main thread: 0x1dd121c40
//2024-02-01 14:50:58.782 I 0x1dd121c40 RUN  /parsing/serialization/node.cpp/benchmark
//2024-02-01 14:50:58.798 I 0x1dd121c40 reading   time is 16224 microseconds
//2024-02-01 14:50:58.963 I 0x1dd121c40 parsing   time is 164997 microseconds
//2024-02-01 14:50:59.219 I 0x1dd121c40 composing time is 255903 microseconds
//2024-02-01 14:50:59.273 I 0x1dd121c40 serialize time is 53718 microseconds
//2024-02-01 14:50:59.273 I 0x1dd121c40 array size: 11351
// UPDATE
//main thread: 0x1df271c40
//2024-02-10 23:44:54.505 I 0x1df271c40 RUN  /parsing/serialization/node.cpp/benchmark_iridium
//2024-02-10 23:44:54.521 I 0x1df271c40 reading   time is 16256 microseconds
//2024-02-10 23:44:54.714 I 0x1df271c40 parsing   time is 192965 microseconds
//2024-02-10 23:44:54.714 I 0x1df271c40 array size: 11351
//2024-02-10 23:44:54.752 I 0x1df271c40 OK   /parsing/serialization/node.cpp/benchmark_iridium
//2024-02-10 23:44:54.752 I 0x1df271c40
//passed: 1
//failed: 0
//total:  23
//-----
//2024-02-01 14:52:04.089 I 0x1dd121c40 RUN  /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 14:52:04.097 I 0x1dd121c40 reading   time is 8010 microseconds
//2024-02-01 14:52:04.136 I 0x1dd121c40 parsing   time is 32176 microseconds - x5 faster !!!
//2024-02-01 14:52:04.136 I 0x1dd121c40 array size: 11351
//2024-02-01 14:52:04.139 I 0x1dd121c40 OK   /parsing/serialization/node.cpp/benchmark_simdjson
//2024-02-01 14:52:04.139 I 0x1dd121c40
//passed: 1
//failed: 0
//total:  23
//-----
//main thread: 0x1da061c40
//2024-02-02 11:51:16.128 I 0x1da061c40 RUN  /parsing/serialization/node.cpp/benchmark_nlohmann
//2024-02-02 11:51:16.144 I 0x1da061c40 reading   time is 15493 microseconds
//2024-02-02 11:51:16.318 I 0x1da061c40 parsing   time is 173638 microseconds x1.02 slower
//2024-02-02 11:51:16.318 I 0x1da061c40 array size: 11351
//2024-02-02 11:51:16.352 I 0x1da061c40 OK   /parsing/serialization/node.cpp/benchmark_nlohmann
//2024-02-02 11:51:16.352 I 0x1da061c40
//passed: 1
//failed: 0
//total:  24
//-----
//MACOS CLANG15 ARM STRING_VIEW
//main thread: 0x1da061c40
//2024-02-02 18:57:23.592 I 0x1da061c40 RUN  /parsing/serialization/node.cpp/benchmark_iridium
//2024-02-02 18:57:23.608 I 0x1da061c40 reading   time is 15946 microseconds
//2024-02-02 18:57:23.738 I 0x1da061c40 parsing   time is 129740 microseconds x1.33 faster
//2024-02-02 18:57:23.762 I 0x1da061c40 OK   /parsing/serialization/node.cpp/benchmark_iridium
//2024-02-02 18:57:23.762 I 0x1da061c40
//passed: 1
//failed: 0
//total:  23
