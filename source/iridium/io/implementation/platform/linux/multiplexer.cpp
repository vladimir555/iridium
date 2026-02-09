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
namespace iridium::io::implementation::platform {


static size_t const DEFAULT_EVENTS_COUNT_LIMIT  = 1024;
static size_t const DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 100;


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
    m_streams_to_add
        (CAsyncQueue<IStream::TSharedPtr>::create()),
    m_streams_to_del
        (CAsyncQueue<IStream::TSharedPtr>::create()),
    m_wake_events
        (CAsyncQueue<Event::TSharedPtr>::create())
{}


void CMultiplexer::initialize() {
//    LOGT << __FUNCTION__;
    LOCK_SCOPE();

    if (m_epoll_fd)
        throw std::runtime_error("multiplexer initializing error: epoll is initialized"); // ----->

    m_is_closing = false;

    m_epoll_fd = epoll_create1(0);
    m_event_fd = eventfd(0, EFD_NONBLOCK);

    struct epoll_event event = {};

    event.events    = EPOLLIN;
    event.data.fd   = m_event_fd;

    assertOK(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_event_fd, &event), "epoll add error");

//    LOGT << __FUNCTION__ << ": " << m_epoll_fd << ", breaker fd " << m_event_fd;
}


void CMultiplexer::finalize() {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer finalization error: epoll is not initialized"); // ----->

    m_is_closing = true;
    uint64_t i = 1;
    assertOK(write(m_event_fd, &i, 8), "multiplexer finalization error: write event_fd error");
    m_epoll_fd = 0;
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    if (!stream || m_is_closing)
        return; // ----->

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer subscribing error: epoll is not initialized"); // ----->

    m_streams_to_add->push(stream);
    eventfd_write(m_event_fd, 1);
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!stream || m_is_closing)
        return;

    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer unsubscribing error: epoll is not initialized"); // ----->

    m_streams_to_del->push(stream);
    eventfd_write(m_event_fd, 1);
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    int epoll_fd = m_epoll_fd.load();
    if (!epoll_fd)
        return {}; // ----->

    std::list<Event::TSharedPtr> events;

    for (auto const &stream: m_streams_to_add->pop(false)) {
        LOCK_SCOPE();
        addInternal(stream);
        events.push_back(
            Event::create(stream, Event::TOperation::OPEN, Event::TStatus::END));
    }

    for (auto const &stream: m_streams_to_del->pop(false)) {
        LOCK_SCOPE();
        delInternal(stream);
        LOGT << "push Event::TOperation::CLOSE, fd: " << stream->getHandles();
        events.push_back(
            Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));
    }

    if (m_is_closing) {
        LOCK_SCOPE();
        if (m_epoll_fd) {
            ::close(m_epoll_fd);
            m_epoll_fd = 0;
        }
        return {}; // ----->
    }

    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    auto count = epoll_wait(
        epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

    if (count > 0) {
        LOCK_SCOPE();
        for (auto i = 0; i < count; i++) {
            if (epoll_events[i].data.fd == m_event_fd) {
                uint64_t val;
            int r = eventfd_read(m_event_fd, &val);
            if (r < 0 && errno != EAGAIN)
                            assertOK(r, "eventfd_read");
                continue; // <---
            }

            auto it = m_map_fd_stream.find(epoll_events[i].data.fd);
            if (it == m_map_fd_stream.end())
                continue;

            auto stream = it->second;

            if (epoll_events[i].events & EPOLLIN)
                events.push_back(
                    Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));

            if (epoll_events[i].events & EPOLLOUT)
                events.push_back(
                    Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));

            if (epoll_events[i].events & (EPOLLHUP | EPOLLRDHUP))
                events.push_back(
                    Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
        }
    }

    events.splice(events.end(), m_wake_events->pop(false));

    return events; // ----->
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer wake error: epoll is not initialized"); // ----->

    m_wake_events->push(event);
    eventfd_write(m_event_fd, 1);
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer wake error: epoll is not initialized"); // ----->

    m_wake_events->push(events);
    eventfd_write(m_event_fd, 1);
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
        int r = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
        if (r < 0 && errno != EEXIST)
            assertOK(r, "epoll add error");

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
            int r = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
            if (r < 0 && errno != ENOENT)
                assertOK(r, "epoll del error");
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


} // iridium::io::implementation::platform


IMPLEMENT_ENUM(iridium::io::implementation::platform::TEpollEvent)


#endif // LINUX_PLATFORM
