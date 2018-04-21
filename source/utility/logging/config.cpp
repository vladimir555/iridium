#include "config.h"


using utility::parsing::implementation::CNode;


namespace utility {
namespace logging {
namespace config {


TLogger createDefaultConsoleLoggerConfig() {
    auto root = CNode::create("logger");

    root->addChild("level", "TRACE_LEVEL");
    root->addChild("console-sink")->addChild("level", "TRACE_LEVEL");

//    if (!file_name.empty()) {
//        auto file_sink = root->addChild("file-sink");
//        file_sink->addChild("level", "TRACE_LEVEL");
//        file_sink->addChild("file-name", file_name);
//    }

    return config::TLogger(root);
}


} // config
} // logger
} // utility
