/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <gtest/gtest.h>
#include <iostream>

#include <iridium/logging/logger.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/threading/thread.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <sys/types.h>

using namespace std;
using iridium::parsing::INode;
using iridium::parsing::implementation::CNode;
using iridium::threading::sleep;


namespace iridium {
namespace logging {


TEST(logging, logger) {
//    cout << "thread id " << getpid() << endl;

//    uint64_t tid;
//    pthread_threadid_np(NULL, &tid);
//    printf("HELLO from thread runnable1 with id : %ld\n", tid);

    INode::TSharedPtr root = CNode::create("logger");
    root->addChild("level", "TRACE_LEVEL");
    root->addChild("console-sink")->addChild("level", "TRACE_LEVEL");
    auto file_sink = root->addChild("file-sink");
    file_sink->addChild("level", "TRACE_LEVEL");
    file_sink->addChild("file-name", ".log");

    config::TLogger logger_config(root);
    Logger::instance().update(logger_config);

    LOGT << "async log trace string via macros, int: " << 5 << " bool: " << true;
    LOGD << "debug";
    LOGI << "info";
    LOGW << "warning";
    LOGE << "error";
    LOGF << "fatal";
    LOGI << "node:\n" << root;

    sleep(100);
}


//iridium async: 1334 ms
//spdlog  async: 1438 ms
//iridium async: 1344 ms
//spdlog  async: 1426 ms
//iridium async: 1847 ms
//spdlog  async: 1573 ms
//iridium async: 1376 ms
//spdlog  async: 1396 ms
//iridium async: 1379 ms
//spdlog  async: 1504 ms
//iridium line : 17:49:23.581 W 140328983201600 string 100000 10.00000 true
//spdlog  line : [2019-11-05 23:49:25.186] [async_file_logger] [warning] string 100000 10 true
//TEST(logging, bench) {
//    config::TLogger config;
//    config::TLogger::TFileSink sink;
//    sink.FileName = "iridium.log";
//    config.FileSink.add(sink);

//    Logger::instance().update(config);

//    LOGT << config.getNode();

//    static int count = 100000;

//    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "spdlog.log");
////    auto async_sink = spdlog::create_async_nb<spdlog::sinks::basic_file_sink_mt>("async_file_logger", "spdlog.log");
////    auto logger = std::make_shared<spdlog::async_logger>
////            ("as", async_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);

//    for (auto nn = 0; nn < 5; nn++) {
//        // iridium logger
//        {
//            double d = 0;
//            int    i = 0;
//            string s = "string";
//            bool   b = true;

//            auto begin = std::chrono::system_clock::now();
//            for (auto n = 0; n < count; n++) {
//                d += 0.0001;
//                i++;
//                b = !b;
//                LOGW << s << " " << i << " " << d << " " << b;
//            }
//            cout << "iridium async: "
//                 << std::chrono::duration_cast<std::chrono::milliseconds>(
//                    std::chrono::system_clock::now() - begin).count()
//                 << " ms" << endl;
////            system("sync");
////            cout << "iridium sync : "
////                 << std::chrono::duration_cast<std::chrono::milliseconds>(
////                    std::chrono::system_clock::now() - begin).count()
////                 << " ms" << endl;

//            sleep(100);
//        }

//        // spdlog logger
//        {
//            double d = 0;
//            int    i = 0;
//            string s = "string";
//            bool   b = true;

//            auto begin = std::chrono::system_clock::now();
//            for (auto n = 0; n < count; n++) {
//                d += 0.0001;
//                i++;
//                b = !b;
//                logger->warn("{} {} {} {}", s, i, d, b);
//            }
//            cout << "spdlog  async: "
//                 << std::chrono::duration_cast<std::chrono::milliseconds>(
//                    std::chrono::system_clock::now() - begin).count()
//                 << " ms" << endl;
////            system("sync");
////            cout << "spdlog  sync : "
////                 << std::chrono::duration_cast<std::chrono::milliseconds>(
////                    std::chrono::system_clock::now() - begin).count()
////                 << " ms" << endl;

//            sleep(100);
//        }
//    }
//}


} // net
} // iridium
