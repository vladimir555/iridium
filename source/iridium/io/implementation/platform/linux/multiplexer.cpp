#include "multiplexer.h"


#ifdef LINUX_PLATFORM


#include <string>
#include <vector>
#include <cstring>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "iridium/convertion/convert.h"
#include "iridium/threading/implementation/async_queue.h"


using iridium::convertion::convert;
using iridium::threading::implementation::CAsyncQueue;
using std::string;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {
namespace platform {


//static size_t DEFAULT_EVENTS_COUNT_LIMIT        = 2;
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 100;


DEFINE_ENUM(
    TEpollEvent,
    EPOLL_IN         = EPOLLIN,
    EPOLL_PRI        = EPOLLPRI,
    EPOLL_OUT        = EPOLLOUT,
    EPOLL_RDNORM     = EPOLLRDNORM,
    EPOLL_RDBAND     = EPOLLRDBAND,
    EPOLL_WRNORM     = EPOLLWRNORM,
    EPOLL_WRBAND     = EPOLLWRBAND,
    EPOLL_MSG        = EPOLLMSG,
    EPOLL_ERR        = EPOLLERR,
    EPOLL_HUP        = EPOLLHUP,
    EPOLL_RDHUP      = EPOLLRDHUP,
    EPOLL_EXCLUSIVE  = EPOLLEXCLUSIVE,
    EPOLL_WAKEUP     = EPOLLWAKEUP,
    EPOLL_ONESHOT    = EPOLLONESHOT,
    EPOLL_ET         = EPOLLET
)


CMultiplexer::CMultiplexer()
:
    m_epoll_fd(0),
    m_event_fd(0),
    m_streams_to_add(CAsyncQueue<IStream::TSharedPtr>::create()),
    m_streams_to_del(CAsyncQueue<IStream::TSharedPtr>::create())
{}


void CMultiplexer::initialize() {
//    LOGT << __FUNCTION__;
    LOCK_SCOPE();

    if (m_epoll_fd)
        throw std::runtime_error("multiplexer initializing error: epoll is initialized"); // ----->

    m_is_closing = false;

    m_epoll_fd = epoll_create(DEFAULT_EVENTS_COUNT_LIMIT);
    m_event_fd = eventfd(0, EFD_NONBLOCK);

    struct epoll_event event = {};

    event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    event.data.fd   = m_event_fd;

    assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event), "epoll add error");

//    LOGT << __FUNCTION__ << ": " << m_epoll_fd << ", breaker fd " << m_event_fd;
}


void CMultiplexer::finalize() {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer finalization error: epoll is not initialized"); // ----->

    m_is_closing = true;
    int64_t i = 0;
    assertOK(write(m_event_fd, &i, 8), "multiplexer finalization error: write event_fd error");
    m_epoll_fd = 0;
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
//    LOGT << "! CMultiplexer::subscribe";
    if (!stream || m_is_closing)
        return; // ----->

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer subscribing error: epoll is not initialized"); // ----->

//    std::const_pointer_cast<IStream>(stream)->initialize();

//    LOGT << __FUNCTION__ << ",   id: " << stream->getID();

    m_streams_to_add->push(stream);
    eventfd_write(m_event_fd, 0);
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!stream || stream->getHandles().empty() || m_is_closing)
        return;

//    LOGT << __FUNCTION__ << ", id: " << stream->getID();

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer unsubscribing error: epoll is not initialized"); // ----->

    m_streams_to_del->push(stream);
    eventfd_write(m_event_fd, 0);
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    if (!m_epoll_fd)
        return {}; // ----->

    LOCK_SCOPE();

    if (m_is_closing) {
//        LOGT << "close epoll";
        ::close(m_epoll_fd);
        m_epoll_fd = 0;
        return {}; // ----->
    }

    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    std::list<Event::TSharedPtr> events;

    for (auto const &stream: m_streams_to_add->pop(false)) {
        addInternal(stream);
        events.push_back(
            Event::create(stream, Event::TOperation::OPEN, Event::TStatus::END));
    }

    for (auto const &stream: m_streams_to_del->pop(false)) {
        delInternal(stream);
        LOGT << "push Event::TOperation::CLOSE, fd: " << stream->getHandles();
        events.push_back(
            Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));
    }

//    LOGT << "wait epoll ...";

    auto count = epoll_wait(
        m_epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

//    LOGT << "wait epoll OK: " << m_epoll_fd << " epoll count " << count;



    for (auto i = 0; i < count; i++) {
//        LOGT << "epoll event: fd " << epoll_events[i].data.fd << " code " <<
//            TEpollEvent(epoll_events[i].events).convertToFlagsString();

//        LOGT << __FUNCTION__ << ",  id: " << epoll_events[i].data.fd << ", flags: " << TEpollEvent(epoll_events[i].events).convertToFlagsString();

        if (epoll_events[i].data.fd == m_event_fd)
            continue; // <---

        if (epoll_events[i].events & EPOLLHUP) {
            events.push_back(
                Event::create(m_map_fd_stream[epoll_events[i].data.fd], Event::TOperation::EOF_, Event::TStatus::BEGIN));
            continue; // <---
        }

        if (epoll_events[i].events & EPOLLIN)
            events.push_back(
                Event::create(m_map_fd_stream[epoll_events[i].data.fd], Event::TOperation::READ, Event::TStatus::BEGIN));

        if (epoll_events[i].events & EPOLLRDHUP) {
            events.push_back(
                Event::create(m_map_fd_stream[epoll_events[i].data.fd], Event::TOperation::CLOSE, Event::TStatus::BEGIN));
            continue; // <---
        }

        if (epoll_events[i].events & EPOLLOUT)
            events.push_back(
                Event::create(m_map_fd_stream[epoll_events[i].data.fd], Event::TOperation::WRITE, Event::TStatus::BEGIN));
    }

    return events; // ----->
}


void CMultiplexer::addInternal(IStream::TSharedPtr const &stream) {
//    if (m_epoll_fd == 0)
//        throw std::runtime_error("epoll add error: not initialized"); // ----->

    for (auto const &fd: stream->getHandles()) {

        if (fd > 0 && m_map_fd_stream.find(fd) == m_map_fd_stream.end()) {
            //        LOGT << m_epoll_fd << " fd " << stream->getID();

            struct epoll_event event = {};

            event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
            event.data.fd   = fd;

            //        LOGT << "add internal: " << stream->getID();
            assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event), "epoll add error");

            m_map_fd_stream[fd] = stream;

            //        // todo: check overflow
            //        eventfd_write(m_event_fd, 0);
        }
    }
}


void CMultiplexer::delInternal(IStream::TSharedPtr const &stream) {
//    if (m_epoll_fd == 0)
//        throw std::runtime_error("epoll add error: not initialized"); // ----->

    for (auto const &fd: stream->getHandles()) {

        //    LOGT << "epoll del: " << m_epoll_fd << " fd " << stream->getID();
        if (fd > 0) {
            assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr), "epoll del error");
            //        stream->finalize();
            m_map_fd_stream.erase(fd);
        }
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
