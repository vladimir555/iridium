/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "multiplexer.h"


#ifdef LINUX_PLATFORM


#include <string>
#include <vector>
#include <cstring>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "iridium/convertion/convert.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/synchronized_scope.h"
#include "iridium/io/implementation/event.h"


using iridium::convertion::convert;
using iridium::threading::implementation::CMutex;
using iridium::threading::Synchronized;
using iridium::threading::implementation::CAsyncQueue;
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


CMultiplexer::CMultiplexer()
:
    m_epoll_fd(0),
    m_event_fd(0),
    m_streams_to_add(CAsyncQueue<IStream::TConstSharedPtr>::create()),
    m_streams_to_del(CAsyncQueue<IStream::TConstSharedPtr>::create())
{}


void CMultiplexer::initialize() {
    LOCK_SCOPE_FAST
    if (m_epoll_fd > 0)
        return; // ----->

//    m_epoll_fd = epoll_create1(0);
    m_epoll_fd = epoll_create(1);
    m_event_fd = eventfd(0, 0);

    struct epoll_event event = {};

    event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
    event.data.fd   = m_event_fd;

    assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event), "epoll add error");

    LOGT << "listener fd " << m_epoll_fd << " breaker fd " << m_event_fd;
}


void CMultiplexer::finalize() {
    LOCK_SCOPE_FAST
    if (m_epoll_fd == 0)
        return; // ----->

    LOGT << m_epoll_fd;
    ::close(m_event_fd);
    ::close(m_epoll_fd);

    m_epoll_fd = 0;
}


void CMultiplexer::subscribe(IStream::TConstSharedPtr const &stream) {
    LOGT << "listener fd " << m_epoll_fd << " add " << stream->getID();
    if (m_epoll_fd == 0)
        throw std::runtime_error("epoll add error: not initialized"); // ----->
    m_streams_to_add->push(stream);
}


void CMultiplexer::unsubscribe(IStream::TConstSharedPtr const &stream) {
    if (m_epoll_fd == 0)
        throw std::runtime_error("epoll del error: not initialized"); // ----->
    m_streams_to_del->push(stream);
}


std::list<IEvent::TSharedPtr> CMultiplexer::waitEvents() {
    LOCK_SCOPE_FAST

    if (m_epoll_fd == 0)
        throw std::runtime_error("epoll wait error: not initialized"); // ----->

    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    for (auto const &stream: m_streams_to_add->pop(false))
        addInternal(stream);

    auto count = epoll_wait(
        m_epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

    LOGT << m_epoll_fd << " epoll count " << count;

    std::list<IEvent::TSharedPtr> events;

    for (auto i = 0; i < count; i++) {
        if (epoll_events[i].data.fd == m_event_fd)
            continue; // <---

        LOGT << m_epoll_fd << " epoll event: fd " << epoll_events[i].data.fd << " code " <<
                TEpollEvent(epoll_events[i].events).convertToFlagsString();

        auto const stream = std::const_pointer_cast<IStream>(m_map_fd_stream[epoll_events[i].data.fd]);

        if (epoll_events[i].events & EPOLLIN)
            events.push_back(CEvent::create(stream, IEvent::TType::READ));

        if (epoll_events[i].events & EPOLLOUT)
            events.push_back(CEvent::create(stream, IEvent::TType::WRITE));
    }

    for (auto const &stream: m_streams_to_del->pop(false))
        delInternal(stream);

    return events; // ----->
}


void CMultiplexer::addInternal(IStream::TConstSharedPtr const &stream) {
    if (m_epoll_fd == 0)
        throw std::runtime_error("epoll add error: not initialized"); // ----->

    if (stream->getID() > 0 && m_map_fd_stream.find(stream->getID()) == m_map_fd_stream.end()) {
        LOGT << m_epoll_fd << " fd " << stream->getID();

        struct epoll_event event = {};

        event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.fd   = stream->getID();

        assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, stream->getID(), &event), "epoll add error");

        m_map_fd_stream[stream->getID()] = stream;

//        // todo: check overflow
//        eventfd_write(m_event_fd, 0);
    }
}


void CMultiplexer::delInternal(IStream::TConstSharedPtr const &stream) {
    if (m_epoll_fd == 0)
        throw std::runtime_error("epoll add error: not initialized"); // ----->

    LOGT << m_epoll_fd << " fd " << stream->getID();
    if (stream->getID() > 0) {
        m_map_fd_stream.erase(stream->getID());
        assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, stream->getID(), nullptr), "epoll del error");
    }
}


int CMultiplexer::assertOK(int const &result, std::string const &message) {
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
