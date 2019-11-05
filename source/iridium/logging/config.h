// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
#define HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A


#include <string>

#include "event.h"
#include "sink.h"

#include "iridium/parsing/serialization/node.h"


namespace iridium {
namespace logging {
namespace config {


DEFINE_ROOT_NODE_BEGIN(Logger)
    DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
    DEFINE_NODE_LIST_BEGIN(ConsoleSink)
        DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
    DEFINE_NODE_LIST_END(ConsoleSink)
    DEFINE_NODE_LIST_BEGIN(FileSink)
        DEFINE_ATTRIBUTE_DEFAULT(TEvent::TLevel, Level, TEvent::TLevel::TRACE_LEVEL)
        DEFINE_ATTRIBUTE_DEFAULT(std::string, FileName, "")
    DEFINE_NODE_LIST_END(FileSink)
DEFINE_ROOT_NODE_END()


TLogger createDefaultConsoleLoggerConfig();


} // config
} // logger
} // iridium


#endif // HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
