// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
#define HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299


#include <string>

#include "iridium/enum.h"


namespace iridium {
namespace logging {


struct TEvent {
    DEFINE_ENUM(
        TLevel,
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR_,
        FATAL
    )
    ///
    TLevel      level;
    ///
    std::string line;
};


} // logger
} // iridium


#endif // HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
