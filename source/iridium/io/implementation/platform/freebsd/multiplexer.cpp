#include "multiplexer.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/worker.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/items.h"
#include "iridium/assert.h"

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>


using std::string;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::nanoseconds;
using iridium::convertion::convert;
using iridium::threading::Synchronized;
using iridium::threading::implementation::CWorker;
using iridium::threading::implementation::CAsyncQueue;


DEFINE_ENUM(
    TEventFlag,
    ADD         = EV_ADD,
    ENABLE      = EV_ENABLE,
    DISABLE     = EV_DISABLE,
    DISPATCH    = EV_DISPATCH,
    DELETE      = EV_DELETE,
    RECEIPT     = EV_RECEIPT,
    ONESHOT     = EV_ONESHOT,
    CLEAR       = EV_CLEAR,
    EOF_        = EV_EOF,
    ERROR_      = EV_ERROR
)


IMPLEMENT_ENUM(TEventFlag)


DEFINE_ENUM(
    TEventFilter,
    READ        = EVFILT_READ,
    WRITE       = EVFILT_WRITE,
    AIO         = EVFILT_AIO,
    VNODE       = EVFILT_VNODE,
    PROC        = EVFILT_PROC,
    SIGNAL      = EVFILT_SIGNAL,
    TIMER       = EVFILT_TIMER,
//    MACHPORT    = EVFILT_MACHPORT,
    FS          = EVFILT_FS,
    USER        = EVFILT_USER,
//    VM          = EVFILT_VM,
//    EXCEPT      = EVFILT_EXCEPT,
    SYSCOUN     = EVFILT_SYSCOUNT
)


IMPLEMENT_ENUM(TEventFilter)


namespace iridium::io::implementation::platform {


void CMultiplexer::handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
};


CMultiplexer::CMultiplexer(std::chrono::microseconds const &timeout)
:
    m_timeout {
        duration_cast<seconds>
            (timeout).count(),
        duration_cast<nanoseconds>
            (timeout).count() %
        duration_cast<nanoseconds>(
            duration_cast<seconds>
                (timeout)).count()
    },
    m_triggered_events
        ( DEFAULT_EVENTS_LIMIT, (struct kevent) { } ),
    m_is_initialized
        (false),
    m_kqueue(0)
{}


void CMultiplexer::initialize() {
//    LOGT << __FUNCTION__ << " ...";
    if (m_kqueue)
        throw std::runtime_error("initialization error: kqueue is not finalized"); // ----->

    struct sigaction signal_handler;
    struct sigaction old_signal_handler;

    // can set to SIG_IGN
    signal_handler.sa_handler   = &handleSignal;
    // restart interrupted system calls
    signal_handler.sa_flags     = SA_RESTART;
    // block every signal during the handler
    sigemptyset(&signal_handler.sa_mask);

    assertOK(sigaction(SIGPIPE, &signal_handler, &old_signal_handler), "sigaction error");

    m_kqueue = assertOK(kqueue(), "kqueue create error");

    try {
        struct kevent event {
            .ident  = static_cast<uintptr_t>(1),
            .filter = EVFILT_USER,
            .flags  = EV_ADD | EV_CLEAR,
            .fflags = 0,
            .data   = 0,
            .udata  = nullptr
        };

        assertOK(
            kevent(m_kqueue, &event, 1, nullptr, 0, nullptr),
        "kevent user registration error");

        m_is_initialized = true;
//        LOGT << "del pipe: " << m_pipe_del[0] << ", add pipe: " << m_pipe_add[0];
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer initialization error: " + string(e.what())); // ----->
    }

//    LOGT << __FUNCTION__ << " OK";
}


void CMultiplexer::finalize() {
    try {
        assertExists(m_kqueue.load(), "kqueue is not initialized");
        LOGT << "finalization begin";
        m_is_initialized = false;
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer finalization error: ") + e.what());
    }
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    if (!m_kqueue)
        return {};

    std::list<Event::TSharedPtr> events;

    auto triggered_event_count = assertOK(
        kevent(
            m_kqueue,
            nullptr, 0,
            m_triggered_events.data(),
            m_triggered_events.size() & std::numeric_limits<int>::max(),
           &m_timeout),
        "kevent waiting event error");

    LOGT << "triggered_event_count: " << triggered_event_count;

    LOCK_SCOPE();

    for (int i = 0; i < triggered_event_count; i++) {
        auto const &triggered_event = m_triggered_events[i];

       LOGT << __FUNCTION__
            << ", id: "     << triggered_event.ident
            << ", flags: "  << TEventFlag(triggered_event.flags).convertToFlagsString()
            << ", flags "   << TEventFlag(triggered_event.flags).convertToFlagsString()
            << ", filter "  << (int16_t)triggered_event.filter
            << ", filter "  << TEventFilter(triggered_event.filter)
            << ", data "    << triggered_event.data
            << ", fflags "  << triggered_event.fflags
            << ", udata "   << (uint64_t)triggered_event.udata;

        if (triggered_event.ident   == 1 &&
            triggered_event.filter  == EVFILT_USER)
        {
            if (!m_is_initialized) {
                close(m_kqueue);
                m_kqueue = 0;

                LOGT << "finalization end";

                return finalizeAllEvents(); // ----->
            }

            std::vector<struct kevent>
                monitored;

            for (auto const &stream: m_streams_to_del->pop(false)) {
                for (auto fd: stream->getHandles()) {
                    // wake event
                    if (fd == 0) {
                        continue; // <---
                    }

                    struct kevent e {
                        .ident  = static_cast<uintptr_t>(fd),
                        .filter = 0,
                        .flags  = EV_DELETE,
                        .fflags = 0,
                        .data   = 0,
                        .udata  = nullptr
                    };

                    auto fd_stream  = m_map_fd_stream.find(fd);
                    if  (fd_stream == m_map_fd_stream.end()) {
                        LOGW << "unsubscribe: fd " << fd << " not in map (already closed)";
                    } else {
                        auto stream = fd_stream->second;
                        auto event  = Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END);
                        m_map_fd_stream.erase(fd_stream);
                        events.push_back(event);

                        e.filter = EVFILT_READ,
                        monitored.push_back(e);
                        e.filter = EVFILT_WRITE;
                        monitored.push_back(e);

                        LOGT << "unsubscribe end event: " << event;
                    }
                }
            }

            for (auto const &stream: m_streams_to_add->pop(false)) {
                for (auto fd: stream->getHandles()) {
                    // wake event
                    if (fd == 0) {
                        continue; // <---
                    }

                    struct kevent e {
                        .ident  = static_cast<uintptr_t>(fd),
                        .filter = 0,
                        .flags  = EV_ADD | EV_CLEAR,
                        .fflags = 0,
                        .data   = 0,
                        .udata  = nullptr
                    };

                    auto fd_stream  = m_map_fd_stream.find(fd);
                    if ( fd_stream == m_map_fd_stream.end()) {
                        auto event  = Event::create(stream, Event::TOperation::OPEN, Event::TStatus::END);

                        m_map_fd_stream[fd] = stream;
                        events.push_back(event);

                        e.filter = EVFILT_READ,
                        monitored.push_back(e);
                        e.filter = EVFILT_WRITE;
                        monitored.push_back(e);

                        LOGT << "subscribe end event: " << event;
                    } else {
                        LOGW << "subscribe: fd " << fd << " already in map";
                    }
                }
            }

            if (monitored.empty())
                continue; // <---

            // Игнорируем ENOENT при удалении — это нормально для уже закрытых дескрипторов
            int result = kevent(m_kqueue, monitored.data(), static_cast<int>(monitored.size()), nullptr, 0, nullptr);
            if (result < 0 && errno != ENOENT) {
                throw std::runtime_error(
                    "kevent update monitored events error: " + string(strerror(errno)));
            }
        } else {
//            LOGT << "get from map fd: " << triggered_event.ident;
            auto fd_stream  = m_map_fd_stream.find(triggered_event.ident);
            if  (fd_stream == m_map_fd_stream.end()) {
                LOGW << "multiplexer waiting events error: kevent not mapped event, fd: "
                     << convert<string>(triggered_event.ident);
                continue;
            }

            auto const &stream = fd_stream->second;
            if (triggered_event.flags & EV_EOF) {
                m_map_fd_stream.erase(fd_stream);
                events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
                continue; // <---
            }

            if (triggered_event.filter == EVFILT_READ)
                events.push_back(Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));

            if (triggered_event.filter == EVFILT_WRITE)
                events.push_back(Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));

            if (triggered_event.flags & EV_ERROR)
                events.push_back(Event::create(stream, Event::TOperation::ERROR_, Event::TStatus::BEGIN));
        }
    }

    events.splice(events.end(), m_wake_events->pop(false));

    return events; // ----->
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    if (!stream || stream->getHandles().empty() || !m_kqueue.load())
        return; // ----->

    try {
        // assertExists(m_kqueue.load(), "kqueue is not initialized");
        m_streams_to_add->push(stream);
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer subscribing error: ") + e.what());
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!stream || stream->getHandles().empty() || !m_kqueue.load())
        return; // ----->

    try {
        // assertExists(m_kqueue.load(), "kqueue is not initialized");
        m_streams_to_del->push(stream);
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer unsubscribing error: ") + e.what());
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    if (!m_kqueue.load())
        return;

    try {
        // assertExists(m_kqueue.load(), "kqueue is not initialized");
        m_wake_events->push(event);
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer waking error: ") + e.what());
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    if (!m_kqueue.load())
        return;

    try {
        assertExists(m_kqueue.load(), "kqueue is not initialized");
        m_wake_events->push(events);
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer waking error: ") + e.what());
    }
}


void CMultiplexer::wakeKEvent() {
    struct kevent trigger {
        .ident  = 1,
        .filter = EVFILT_USER,
        .flags  = 0,
        .fflags = NOTE_TRIGGER,
        .data   = 0,
        .udata  = nullptr
    };

    kevent(m_kqueue, &trigger, 1, nullptr, 0, nullptr);
}


} // iridium::io::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM
