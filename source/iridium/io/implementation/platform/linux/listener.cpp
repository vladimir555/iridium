/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


#ifdef LINUX_PLATFORM


#include <string>
#include <vector>
#include <cstring>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "iridium/convertion/convert.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/synchronized_scope.h"


using iridium::convertion::convert;
using iridium::threading::implementation::CMutex;
using iridium::threading::Synchronized;
using std::string;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {
namespace platform {


//static size_t DEFAULT_EVENTS_COUNT_LIMIT        = 2;
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 1000;


DEFINE_ENUM(
    TEpollEvent,
    EPOLLIN         = EPOLL_EVENTS::EPOLLIN,
    EPOLLPRI        = EPOLL_EVENTS::EPOLLPRI,
    EPOLLOUT        = EPOLL_EVENTS::EPOLLOUT,
    EPOLLRDNORM     = EPOLL_EVENTS::EPOLLRDNORM,
    EPOLLRDBAND     = EPOLL_EVENTS::EPOLLRDBAND,
    EPOLLWRNORM     = EPOLL_EVENTS::EPOLLWRNORM,
    EPOLLWRBAND     = EPOLL_EVENTS::EPOLLWRBAND,
    EPOLLMSG        = EPOLL_EVENTS::EPOLLMSG,
    EPOLLERR        = EPOLL_EVENTS::EPOLLERR,
    EPOLLHUP        = EPOLL_EVENTS::EPOLLHUP,
    EPOLLRDHUP      = EPOLL_EVENTS::EPOLLRDHUP,
    EPOLLEXCLUSIVE  = EPOLL_EVENTS::EPOLLEXCLUSIVE,
    EPOLLWAKEUP     = EPOLL_EVENTS::EPOLLWAKEUP,
    EPOLLONESHOT    = EPOLL_EVENTS::EPOLLONESHOT,
    EPOLLET         = EPOLL_EVENTS::EPOLLET
)


CListener::CListener()
:
    m_epoll_fd(0),
    m_event_fd(0)
{}


void CListener::initialize() {
//    m_epoll_fd = epoll_create1(0);
    m_epoll_fd = epoll_create(1);
//    m_event_fd = eventfd(0, 0);

//    struct epoll_event event = {};

//    event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
//    event.data.fd   = m_event_fd;

//    assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event), "epoll add error");

    LOGT;
}


void CListener::finalize() {
    LOGT;
    ::close(m_epoll_fd);
}


void CListener::add(IStream::TSharedPtr const &stream) {
    if (stream->getID() > 0 && m_map_fd_stream.find(stream->getID()) == m_map_fd_stream.end()) {
        LOGT << "fd " << stream->getID();

        struct epoll_event event = {};

        event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.fd   = stream->getID();

        assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, stream->getID(), &event), "epoll add error");

        m_map_fd_stream[stream->getID()] = stream;
    }
}


void CListener::del(IStream::TSharedPtr const &stream) {
    LOGT << "fd " << stream->getID();
    if (stream->getID() > 0) {
        m_map_fd_stream.erase(stream->getID());
        assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, stream->getID(), nullptr), "epoll del error");
    }
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
        if (epoll_events[i].data.fd == m_event_fd)
            continue; // <---

        LOGT << "epoll event: fd " << epoll_events[i].data.fd << " code " << TEpollEvent(epoll_events[i].events).convertToFlagsString();
        if (epoll_events[i].events & EPOLLIN)
            events.push_back(Event::create(Event::TType::READ,  m_map_fd_stream[epoll_events[i].data.fd]));

        if (epoll_events[i].events & EPOLLOUT)
            events.push_back(Event::create(Event::TType::WRITE, m_map_fd_stream[epoll_events[i].data.fd]));
    }

    return events; // ----->
}


int CListener::assertOK(int const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(
            message + ", " + std::strerror(errno)  + ", code " + convert<string>(errno)); // ----->
    else
        return result; // ----->
}


} // platform
} // implementation
} // io
} // iridium


IMPLEMENT_ENUM(iridium::io::implementation::platform::TEpollEvent)


#endif // LINUX_PLATFORM
