// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
#define HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A


#include <string>

#include "event.h"
#include "sink.h"

#include "iridium/parsing/serialization/node.h"
#include "iridium/io/uri.h"


namespace iridium::logging::config {


DEFINE_ROOT_NODE_BEGIN(Logger)
    DEFINE_ATTRIBUTE(bool, IsGmtTime, false)
    DEFINE_ATTRIBUTE(TEvent::TLevel, Level, TEvent::TLevel::TRACE)
    DEFINE_NODE_LIST_BEGIN(Sink)
        // todo: ratate (none, hourly, dayly, weekly, onstart, continuously) + url (file://...  , postgres:// ...)
        DEFINE_ENUM(
            TSinkType,
            CONSOLE,
            FILE,
            FILE_DAILY, // rotate daily
            FILE_TIMELY) // new file every logger starting
        DEFINE_ATTRIBUTE(TSinkType      , Type      , TSinkType::UNKNOWN)
        DEFINE_ATTRIBUTE(bool           , IsAsync   , false)
        DEFINE_ATTRIBUTE(std::string    , Uri       , "")
        DEFINE_ATTRIBUTE(TEvent::TLevel , Level     , TEvent::TLevel::UNKNOWN)
    DEFINE_NODE_LIST_END(Sink)
DEFINE_ROOT_NODE_END()


TLogger createDefault();


} // namespace iridium::logging::config


//DEFINE_ENUM_CONVERT(iridium::logging::config::TLogger::TSink::TSinkType)


#endif // HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
