#include "sink.h"

#include "iridium/convertion/convert.h"
#include "iridium/threading/thread.h"

#include <chrono>


IMPLEMENT_ENUM(iridium::logging::ISink::TType)


using iridium::convertion::convert;
using std::string;
using std::chrono::system_clock;
using iridium::threading::getThreadID;


namespace iridium {
namespace logging {


std::atomic<bool> ISink::is_gmt_time(false);


std::string ISink::makeLine(TEvent::TConstSharedPtr const &e) {
    return
        convert<string>(e->time, static_cast<bool>(is_gmt_time)) + " " +
        convert<string>(e->level).substr(0, 1) + " " +
  rjust(convert<string>(e->thread_id), 5, ' ') + " " +
        e->line; // ----->
}


} // logger
} // iridium
