/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


#ifdef LINUX_PLATFORM


#include <string>
#include <vector>
#include <sys/epoll.h>
#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


//static int assertOK(int const &result, std::string const &error_message) {
//}


static size_t DEFAULT_EVENTS_COUNT_LIMIT        = 2;
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 1000;


CListener::CListener()
:
    m_epoll_fd(0)
{}


void CListener::initialize() {
    m_epoll_fd = epoll_create1(0);
}


void CListener::finalize() {
}


void CListener::add(IStreamPort::TSharedPtr const &stream) {
    struct epoll_event event = {};

    event.events    = EPOLLIN | EPOLLET | EPOLLOUT;
    event.data.fd   = stream->getID();

//    auto result     =
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, stream->getID(), &event);

    m_map_fd_stream[stream->getID()] = stream;
}


void CListener::del(IStreamPort::TSharedPtr const &/*stream*/) {
}


CListener::TEvents CListener::wait() {
    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    auto count = epoll_wait(
        m_epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

    LOGT << "epoll count " << count;

    CListener::TEvents events;

    for (auto i = 0; i < count; i++) {
        auto event = Event::create(Event::TType::READ, m_map_fd_stream[epoll_events[i].data.fd]);
        events.push_back(event);
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM
