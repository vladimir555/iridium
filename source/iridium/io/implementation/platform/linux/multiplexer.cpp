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


//static size_t DEFAULT_EVENTS_COUNT_LIMIT        = 2;
static size_t DEFAULT_EVENTS_WAITING_TIMEOUT_MS = 1000;


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
    m_event_fd(0)
{}


void CMultiplexer::initialize() {
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
}


void CMultiplexer::finalize() {
    if (!m_epoll_fd)
        throw std::runtime_error("multiplexer finalization error: epoll is not initialized"); // ----->

    {
        LOCK_SCOPE();
        m_is_closing = true;
    }

    eventfd_write(m_event_fd, 1);
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    if (!stream || m_is_closing || !m_epoll_fd)
        return; // ----->

    m_wake_events->push(Event::create(stream, Event::TOperation::OPEN, Event::TStatus::BEGIN));
    eventfd_write(m_event_fd, 1);
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!stream || m_is_closing || !m_epoll_fd)
        return; // ----->

    m_wake_events->push(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
    eventfd_write(m_event_fd, 1);
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    int epoll_fd = m_epoll_fd;
    if (!epoll_fd)
        return {}; // ----->

    std::list<Event::TSharedPtr> events;

    {
        LOCK_SCOPE();
        if (m_is_closing) {
            ::close(m_epoll_fd);
            ::close(m_event_fd);
            m_epoll_fd = 0;
            m_event_fd = 0;

            auto events = finalizeAllEvents();
            return events;
        }

        // Process wake events first to handle subscriptions/unsubscriptions before polling
        auto wake_events = m_wake_events->pop(false);
        // Deletions first to handle FD reuse correctly
        for (auto const &event: wake_events) {
            if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
                delInternal(event->stream);
            }
        }
        // Then additions and other events
        for (auto const &event: wake_events) {
            if (event->operation == Event::TOperation::OPEN && event->status == Event::TStatus::BEGIN) {
                addInternal(event->stream);
                event->status = Event::TStatus::END;
                events.push_back(event);
            } else if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
                event->status = Event::TStatus::END;
                events.push_back(event);
            } else {
                events.push_back(event);
            }
        }
    }

    struct epoll_event epoll_events[DEFAULT_EVENTS_COUNT_LIMIT];

    auto count = epoll_wait(
        epoll_fd,
        epoll_events,
        DEFAULT_EVENTS_COUNT_LIMIT,
        DEFAULT_EVENTS_WAITING_TIMEOUT_MS);

    {
        LOCK_SCOPE();
        for (auto i = 0; i < count; i++) {
            if (epoll_events[i].data.fd == m_event_fd) {
                eventfd_t val;
                eventfd_read(m_event_fd, &val);
                auto wake_events_2 = m_wake_events->pop(false);
                for (auto const &event: wake_events_2) {
                    if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
                        delInternal(event->stream);
                    }
                }
                for (auto const &event: wake_events_2) {
                    if (event->operation == Event::TOperation::OPEN && event->status == Event::TStatus::BEGIN) {
                        addInternal(event->stream);
                        event->status = Event::TStatus::END;
                        events.push_back(event);
                    } else if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
                        event->status = Event::TStatus::END;
                        events.push_back(event);
                    } else {
                        events.push_back(event);
                    }
                }
                continue;
            }

            auto fd_stream  = m_map_fd_stream.find(epoll_events[i].data.fd);
            if ( fd_stream == m_map_fd_stream.end())
                continue;

            auto stream = fd_stream->second;

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

    return events; // ----->
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    m_wake_events->push(event);
    if (m_epoll_fd)
        eventfd_write(m_event_fd, 1);
}


bool CMultiplexer::isFinalized() const {
    return m_epoll_fd == 0;
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    m_wake_events->push(events);
    if (m_epoll_fd)
        eventfd_write(m_event_fd, 1);
}


void CMultiplexer::addInternal(IStream::TSharedPtr const &stream) {
    for (auto const &fd: stream->getHandles()) {
        if (fd > 0) {
            struct epoll_event event = {};

            event.events    = EPOLLERR | EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
            event.data.fd   = fd;

            auto result = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
            if (result < 0 && errno == EEXIST)
                result = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &event);

            assertOK(result, "epoll add/mod error");

            m_map_fd_stream[fd] = stream;
        }
    }
}


void CMultiplexer::delInternal(IStream::TSharedPtr const &stream) {
    for (auto const &fd: stream->getHandles()) {
        if (fd > 0) {
            m_map_fd_stream.erase(fd);
            auto result = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
            if ( result < 0 && errno != ENOENT)
                assertOK(result, "epoll del error");
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
