/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


#ifdef WINDOWS_PLATFORM


//#include <string>
//#include <vector>
//#include <sys/epoll.h>
//#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


static size_t DEFAULT_EVENTS_COUNT_LIMIT        = 2;
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 1000;


CListener::CListener() {
    throw std::runtime_error("windows async io not implemented");
}


void CListener::initialize() {
    throw std::runtime_error("windows async io not implemented");
}


void CListener::finalize() {
    throw std::runtime_error("windows async io not implemented");
}


void CListener::add(IStreamPort::TSharedPtr const &stream) {
    throw std::runtime_error("windows async io not implemented");
}


void CListener::del(IStreamPort::TSharedPtr const &/*stream*/) {
    throw std::runtime_error("windows async io not implemented");
}


CListener::TEvents CListener::wait() {
    throw std::runtime_error("windows async io not implemented");
}


} // platform
} // implementation
} // io
} // iridium


#endif // WINDOWS_PLATFORM
