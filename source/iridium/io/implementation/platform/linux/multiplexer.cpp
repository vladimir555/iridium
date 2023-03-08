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
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 5000;


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
    Synchronized(CMutex::create()),
    m_epoll_fd(0),
    m_event_fd(0),
    m_streams_to_add(CAsyncQueue<IStream::TConstSharedPtr>::create()),
    m_streams_to_del(CAsyncQueue<IStream::TConstSharedPtr>::create())
{}


void CMultiplexer::initialize() {
//    LOGT << __FUNCTION__;
    LOCK_SCOPE_FAST

    if (m_epoll_fd)
        throw std::runtime_error("multiplexer initializing error: epoll is initialized"); // ----->

    m_epoll_fd = epoll_create(DEFAULT_EVENTS_COUNT_LIMIT);
    m_event_fd = eventfd(0, EFD_NONBLOCK);

    struct epoll_event event = {};

    event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
    event.data.fd   = m_event_fd;

    assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event), "epoll add error");

//    LOGT << __FUNCTION__ << ": " << m_epoll_fd << ", breaker fd " << m_event_fd;
}


void CMultiplexer::finalize() {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer finalizing error: epoll is not initialized"); // ----->

//    LOGT << "notify close epoll";
    int64_t i = 0;
    write(m_event_fd, &i, 8);
//    LOGT << "close epoll";
    ::close(m_epoll_fd);
    m_epoll_fd = 0;
}


void CMultiplexer::subscribe(IStream::TConstSharedPtr const &stream) {
    if (!stream || !stream->getID())
        return;

//    LOGT << __FUNCTION__ << ": " << m_epoll_fd << " " << stream->getID();

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer subscribing error: epoll is not initialized"); // ----->

    m_streams_to_add->push(stream);
    eventfd_write(m_event_fd, 0);
}


void CMultiplexer::unsubscribe(IStream::TConstSharedPtr const &stream) {
    if (!stream || !stream->getID())
        return;

//    LOGT << __FUNCTION__ << ": " << m_epoll_fd << " " << stream->getID();

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer unsubscribing error: epoll is not initialized"); // ----->

    m_streams_to_del->push(stream);
    eventfd_write(m_event_fd, 0);
}


std::list<IEvent::TSharedPtr> CMultiplexer::waitEvents() {
    if (!m_epoll_fd)
        return {};
//    if (!m_epoll_fd)
//        throw std::runtime_error("multiplexer wait events error: epoll is not initialized"); // ----->

    LOCK_SCOPE_FAST

    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    for (auto const &stream: m_streams_to_add->pop(false))
        addInternal(stream);

    for (auto const &stream: m_streams_to_del->pop(false))
        delInternal(stream);

//    LOGT << "wait epoll ...";

    auto count = epoll_wait(
        m_epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

//    LOGT << "wait epoll OK: " << m_epoll_fd << " epoll count " << count;

    std::list<IEvent::TSharedPtr> events;

    for (auto i = 0; i < count; i++) {
//        LOGT << m_epoll_fd << " epoll event: fd " << epoll_events[i].data.fd << " code " <<
//            TEpollEvent(epoll_events[i].events).convertToFlagsString();

        if (epoll_events[i].data.fd == m_event_fd)
            continue; // <---

        if (epoll_events[i].events & EPOLLIN)
            events.push_back(
                CEvent::create(std::const_pointer_cast<IStream>(m_map_fd_stream[epoll_events[i].data.fd]), IEvent::TType::READ));

        if (epoll_events[i].events & EPOLLOUT)
            events.push_back(
                CEvent::create(std::const_pointer_cast<IStream>(m_map_fd_stream[epoll_events[i].data.fd]), IEvent::TType::WRITE));
    }

    return events; // ----->
}


void CMultiplexer::addInternal(IStream::TConstSharedPtr const &stream) {
//    if (m_epoll_fd == 0)
//        throw std::runtime_error("epoll add error: not initialized"); // ----->

    if (stream->getID() > 0 && m_map_fd_stream.find(stream->getID()) == m_map_fd_stream.end()) {
//        LOGT << m_epoll_fd << " fd " << stream->getID();

        struct epoll_event event = {};

        event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.fd   = stream->getID();

//        LOGT << "add internal: " << stream->getID();
        assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, stream->getID(), &event), "epoll add error");

        m_map_fd_stream[stream->getID()] = stream;

//        // todo: check overflow
//        eventfd_write(m_event_fd, 0);
    }
}


void CMultiplexer::delInternal(IStream::TConstSharedPtr const &stream) {
//    if (m_epoll_fd == 0)
//        throw std::runtime_error("epoll add error: not initialized"); // ----->

//    LOGT << "epoll del: " << m_epoll_fd << " fd " << stream->getID();
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
