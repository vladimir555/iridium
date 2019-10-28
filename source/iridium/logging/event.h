#ifndef HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
#define HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299


#include <string>

#include "iridium/enum.h"


namespace iridium {
namespace logging {


struct TEvent {
    DEFINE_ENUM(
        TLevel,
        TRACE_LEVEL,
        DEBUG_LEVEL,
        INFO_LEVEL,
        WARNING_LEVEL,
        ERROR_LEVEL,
        FATAL_LEVEL
    )
    ///
    TLevel      level;
    ///
    std::string line;
};


} // logger
} // iridium


#endif // HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
