// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
#define HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299


#include <string>

#include "iridium/enum.h"
#include "iridium/smart_ptr.h"


namespace iridium {
namespace logging {


struct TEvent {
    DEFINE_CREATE(TEvent)
    DEFINE_ENUM(
        TLevel,
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR_,
        FATAL
    )
    std::chrono::system_clock::time_point const
        time;
    TLevel const
        level;
    std::thread::id const
        thread_id;
    std::string
        line;
};


} // logger
} // iridium


DEFINE_ENUM_CONVERT(iridium::logging::TEvent::TLevel)


#endif // HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
