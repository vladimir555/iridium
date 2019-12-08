/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


#ifdef LINUX_PLATFORM


#include <string>
#include <vector>

#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


// todo: assert !!!
//static int assertOK(int const &result, std::string const &error_message) {
//}


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

    event.events    = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    event.data.fd   = stream->getID();

//    auto result     =
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, stream->getID(), &event);

    m_map_fd_stream[stream->getID()] = stream;
}


void CListener::del(IStreamPort::TSharedPtr const &stream) {
    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, stream->getID(), nullptr);
}


CListener::TEvents CListener::wait() {
    auto count = epoll_wait(
        m_epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

    LOGT << "epoll count " << count;

    CListener::TEvents events;

    for (auto i = 0; i < count; i++) {
        LOGT << "epoll event: " << epoll_events[i].events;

        Event::TType type;

        if (epoll_events[i].events & EPOLLIN)
            type    = Event::TType::WRITE;
        if (epoll_events[i].events & EPOLLOUT)
            type    = Event::TType::READ;
        if (epoll_events[i].events & EPOLLRDHUP) {
            type    = Event::TType::CLOSE;
         }

        events.push_back(Event::create(type, m_map_fd_stream[epoll_events[i].data.fd]));
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM
