#include "config.h"

#include <mutex>


using iridium::parsing::implementation::CNode;


IMPLEMENT_ENUM(iridium::logging::config::TLogger::TSink::TSinkType)


namespace iridium::logging::config {


TLogger createDefault() {
    auto root = CNode::create("logger");

//    root->addChild("level", "TRACE");
    root->addChild("sink")->addChild("type", "console");

//    if (!file_name.empty()) {
//        auto file_sink = root->addChild("file-sink");
//        file_sink->addChild("level", "TRACE_LEVEL");
//        file_sink->addChild("file-name", file_name);
//    }

    return config::TLogger(root);
}


} // namespace iridium::logging::config
