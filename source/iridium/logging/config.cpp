#include "config.h"

#include <mutex>


using iridium::parsing::implementation::CNode;


IMPLEMENT_ENUM(iridium::logging::config::TLogger::TSink::TSinkType)


namespace iridium {
namespace logging {
namespace config {


TLogger createDefault() {
    auto root = CNode::create("logger");

    root->addChild("level", "TRACE_LEVEL");
    root->addChild("sink")->addChild("type", "console");

//    if (!file_name.empty()) {
//        auto file_sink = root->addChild("file-sink");
//        file_sink->addChild("level", "TRACE_LEVEL");
//        file_sink->addChild("file-name", file_name);
//    }

    return config::TLogger(root);
}


} // config
} // logger
} // iridium
