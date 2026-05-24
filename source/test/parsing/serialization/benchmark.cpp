#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>
#include <iridium/parsing/implementation/json_parser.h>
#include <iridium/io/fs/files.h>
// #include <simdjson.h>
// #include <nlohmann/json.hpp>

#include <string>

// #include <valgrind/callgrind.h>


using std::string;
using namespace iridium::parsing;


//struct S1 {
//    struct S2 {
//        S2(S1 const * const s1) {};
//    } s2 = this;
//
//} s1;


// https://raw.githubusercontent.com/json-iterator/test-data/refs/heads/master/large-file.json
DEFINE_ROOT_NODE_BEGIN(Root, iridium::parsing::serialization::TNamingStrategyCPPToNode::CAMEL_CASE_TO_LOWER_KEBAB)
    DEFINE_NODE_LIST_BEGIN(Array)
        DEFINE_ATTRIBUTE(uint64_t, Id)
        DEFINE_ATTRIBUTE(string, Type)
        DEFINE_NODE_BEGIN(Actor)
            DEFINE_ATTRIBUTE(uint64_t, Id)
            DEFINE_ATTRIBUTE(string, Login)
            DEFINE_ATTRIBUTE(string, Url)
            DEFINE_ATTRIBUTE(string, AvatarUrl, "")
        DEFINE_NODE_END(Actor)
        DEFINE_NODE_BEGIN(Repo)
            DEFINE_ATTRIBUTE(uint64_t, Id)
            DEFINE_ATTRIBUTE(string, Name)
            DEFINE_ATTRIBUTE(string, Url)
        DEFINE_NODE_END(Repo)
        DEFINE_NODE_BEGIN(Payload)
            DEFINE_ATTRIBUTE(uint64_t, PushId, 0)
            DEFINE_ATTRIBUTE(uint64_t, Size, 0)
            DEFINE_ATTRIBUTE(uint64_t, DistinctSize, 0)
            DEFINE_ATTRIBUTE(string, Ref, "")
            DEFINE_ATTRIBUTE(string, RefType, "")
            DEFINE_ATTRIBUTE(string, MasterBranch, "")
            DEFINE_ATTRIBUTE(string, PusherType, "")
            DEFINE_NODE_LIST_BEGIN(Commits)
                DEFINE_ATTRIBUTE(string, Sha)
                DEFINE_NODE_BEGIN(Author)
                    DEFINE_ATTRIBUTE(string, Name, "")
                    DEFINE_ATTRIBUTE(string, Email)
                DEFINE_NODE_END(Author)
            DEFINE_NODE_LIST_END(Commits)
        DEFINE_NODE_END(Payload)
//    DEFINE_ATTRIBUTE(std::chrono::system_clock::time_point, CreatedAt)
    DEFINE_NODE_LIST_END(Array)
DEFINE_ROOT_NODE_END();


// https://raw.githubusercontent.com/json-iterator/test-data/refs/heads/master/large-file.json
struct TJsonRoot {
   struct TItem {
       uint64_t    Id;
       string      Type;

       struct TActor {
           uint64_t    Id;
           string      Login;
           string      Url;
           string      AvatarUrl;
       };
       TActor Actor;

       struct TRepo {
           uint64_t    Id;
           string      Name;
           string      Url;
       };
       TRepo Repo;

       struct TPayload {
           uint64_t    PushId;
           uint64_t    Size;
           uint64_t    DistinctSize;
           string      Ref;
           string      RefType;
           string      MasterBranch;
           string      PusherType;

           struct TCommit {
               string Sha;

               struct TAuthor {
                   string Name;
                   string Email;
               };
               TAuthor Author;
           };
           std::list<TCommit> Commits;
       };
       TPayload    Payload;
       std::chrono::system_clock::time_point
                   CreatedAt;
   };
   std::list<TItem> Array;
};


// TEST(benchmark_1_nlohmann) {
//     if (!iridium::io::fs::checkFileExistence("sample.json")) {
//         LOGI << "skip";
//         return;
//     }

//     auto now        = std::chrono::system_clock::now();
//     // https://raw.githubusercontent.com/json-iterator/test-data/refs/heads/master/large-file.json
//     auto json_text  = iridium::io::fs::readFile("sample.json");

//     TJsonRoot root = { };

//     LOGI << "reading   time is " << std::chrono::system_clock::now() - now;
//     now = std::chrono::system_clock::now();

//     auto document   = nlohmann::json::parse(json_text);

//     for (auto const &json_item: document) {
//         TJsonRoot::TItem item {
//             .Id     = std::stoull(json_item["id"].get<std::string>()),
//             .Type   = json_item["type"].get<std::string>(),
//             .Actor  = TJsonRoot::TItem::TActor {
//                 .Id     = std::stoull(json_item["id"].get<std::string>()),
//                 .Login      = json_item["actor"]["login"].get<std::string>(),
//                 .Url        = json_item["actor"]["url"].get<std::string>(),
//                 .AvatarUrl  = json_item["actor"]["avatar_url"].get<std::string>(),
//             },
//             .Repo       = TJsonRoot::TItem::TRepo {
//                 .Id         = json_item["repo"]["id"].get<uint64_t>(),
//                 .Name       = json_item["repo"]["name"].get<std::string>(),
//                 .Url        = json_item["repo"]["url"].get<std::string>(),
//             },
//             .Payload    = TJsonRoot::TItem::TPayload {
//                 .PushId         = json_item["payload"].value("push_id", uint64_t(0)),
//                 .Size           = json_item["payload"].value("size", uint64_t(0)),
//                 .DistinctSize   = json_item["payload"].value("distinct", uint64_t(0)),
//                 .Ref            = json_item["payload"]["ref"].dump(),
//                 .RefType        = json_item["payload"].value("ref_type", ""),
//                 .MasterBranch   = json_item["payload"].value("master_branch", ""),
//                 .PusherType     = json_item["payload"].value("pusher_type", ""),
//             }
//         };

//         if (json_item["payload"]["commits"].is_array()) {
//             for (auto const &json_commit: json_item["payload"]["commits"]) {
//                 item.Payload.Commits.push_back(
//                     TJsonRoot::TItem::TPayload::TCommit {
//                         .Sha    = json_commit["sha"].get<std::string>(),
//                         .Author = TJsonRoot::TItem::TPayload::TCommit::TAuthor {
//                             .Name   = json_commit["author"]["name"].get<std::string>(),
//                             .Email  = json_commit["author"]["email"].get<std::string>(),
//                         }
//                     }
//                 );
//             }
//         }

//         if (json_item["payload"]["commits"].is_object()) {
//             auto const &json_commit = json_item["payload"]["commits"];
//             item.Payload.Commits.push_back(
//                 TJsonRoot::TItem::TPayload::TCommit {
//                     .Sha    = json_commit["sha"].get<std::string>(),
//                     .Author = TJsonRoot::TItem::TPayload::TCommit::TAuthor {
//                         .Name   = json_commit["author"]["name"].get<std::string>(),
//                         .Email  = json_commit["author"]["email"].get<std::string>(),
//                     }
//                 }
//             );
//         }

//         root.Array.push_back(item);
//     }

//     LOGI << "parsing   time is " << std::chrono::system_clock::now() - now;
//     LOGI << "array size: " << root.Array.size();
// }


// apple m1, input json 25 MB
// C++
//2025-03-10 17:03:38.084 I 0x1ee6d8840 RUN  /parsing/serialization/node.cpp/benchmark_iridium
//2025-03-10 17:03:38.099 I 0x1ee6d8840 reading   time is 15042 microseconds
//2025-03-10 17:03:38.243 I 0x1ee6d8840 parsing   time is 143888 microseconds
//2025-03-10 17:03:38.488 I 0x1ee6d8840 composing time is 244827 microseconds
//2025-03-10 17:03:38.516 I 0x1ee6d8840 serialize time is 27635 microseconds
//2025-03-10 17:03:38.516 I 0x1ee6d8840 array size: 11351
//2025-03-10 17:03:38.558 I 0x1ee6d8840 OK   /parsing/serialization/node.cpp/benchmark_iridium
//2025-03-10 17:03:38.558 I 0x1ee6d8840 RUN  /parsing/serialization/node.cpp/benchmark_nlohmann
//2025-03-10 17:03:38.572 I 0x1ee6d8840 reading   time is 13727 microseconds
//2025-03-10 17:03:38.735 I 0x1ee6d8840 parsing   time is 162845 microseconds
//2025-03-10 17:03:38.735 I 0x1ee6d8840 array size: 11351
//2025-03-10 17:03:38.769 I 0x1ee6d8840 OK   /parsing/serialization/node.cpp/benchmark_nlohmann
//2025-03-10 17:03:38.769 I 0x1ee6d8840
TEST(benchmark_2_iridium) {
    if (!iridium::io::fs::checkFileExistence("sample.json")) {
        LOGI << "'sample.json' not found, skip";
        return;
    }

    using implementation::CJSONParser;
    using iridium::io::fs::readFile;
    using iridium::io::fs::writeFile;

    auto now        = std::chrono::system_clock::now();
    auto parser     = CJSONParser::create();
    // https://raw.githubusercontent.com/json-iterator/test-data/refs/heads/master/large-file.json
    auto json_text  = readFile("sample.json");
    LOGI << "reading     time is " << std::chrono::system_clock::now() - now;

    now = std::chrono::system_clock::now();
    auto node = parser->parse(json_text);
////    benchmark::TRoot json_serialized(node);
    LOGI << "parsing     time is " << std::chrono::system_clock::now() - now;

    now = std::chrono::system_clock::now();
    // CALLGRIND_START_INSTRUMENTATION;
    TRoot json_serialized(node);
    // CALLGRIND_STOP_INSTRUMENTATION;
    LOGI << "deserialize time is " << std::chrono::system_clock::now() - now;
    LOGI << "array size: " << json_serialized.Array.size();

    now = std::chrono::system_clock::now();
    auto json_string = parser->compose(node);
    LOGI << "composing   time is " << std::chrono::system_clock::now() - now;

    now = std::chrono::system_clock::now();
    writeFile("sample.composed.json", json_string);
    LOGI << "writing     time is " << std::chrono::system_clock::now() - now;
}


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
