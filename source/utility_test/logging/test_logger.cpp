#include <gtest/gtest.h>
#include <iostream>

#include <utility/logging/logger.h>
#include <utility/parsing/implementation/node.h>
#include <utility/threading/thread.h>

#include <sys/types.h>

using namespace std;
using utility::parsing::INode;
using utility::parsing::implementation::CNode;
using utility::threading::sleep;


namespace utility {
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


} // networking
} // utility
