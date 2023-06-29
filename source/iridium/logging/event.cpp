#include "event.h"


IMPLEMENT_ENUM(iridium::logging::TEvent::TLevel)


namespace iridium {
namespace logging {


TEvent::TEvent(TLevel const &level_, std::string const &line_)
:
    level   (level_),
    line    (line_)
{}


} // logger
} // iridium
