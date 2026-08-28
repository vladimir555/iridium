// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "sink.h"

#include "iridium/convertion/convert.h"
#include "iridium/threading/thread.h"

#include <chrono>


// IMPLEMENT_ENUM(iridium::logging::ISink::TType)


using iridium::convertion::convert;
using std::string;
using std::chrono::system_clock;
using iridium::threading::getThreadID;


namespace iridium::logging {


std::atomic<bool> ISink::is_gmt_time(false);


std::string ISink::makeLine(TEvent::TConstSharedPtr const &e) {
    return
        convert<string>(e->time, static_cast<bool>(is_gmt_time)) + " " +
        convert<string>(e->level).substr(0, 1) + " " +
  rjust(convert<string>(e->thread_id), 5, ' ') + " " +
        e->line; // ----->
}


} // namespace iridium::logging
