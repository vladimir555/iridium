#ifndef HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
#define HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A


#include <string>

#include "event.h"
#include "sink.h"

#include "utility/parsing/serialization/node.h"


namespace utility {
namespace logging {
namespace config {


DEFINE_ROOT_NODE_BEGIN(Logger)
    DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
    DEFINE_NODE_LIST_BEGIN(ConsoleSink)
        DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
    DEFINE_NODE_LIST_END(ConsoleSink)
    DEFINE_NODE_LIST_BEGIN(FileSink)
        DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
        DEFINE_ATTRIBUTE(std::string, FileName)
    DEFINE_NODE_LIST_END(FileSink)
DEFINE_ROOT_NODE_END()


} // config
} // logger
} // utility


#endif // HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
