#include "multiplexer.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/worker.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/items.h"
#include "iridium/assert.h"

// fallback to poll multiplexer for freebsd kevent pipe bug workaround
#include "../unix/multiplexer.h"

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>


using std::string;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::nanoseconds;
using iridium::convertion::convert;
using iridium::threading::Synchronized;
using iridium::threading::implementation::CWorker;
using iridium::threading::implementation::CAsyncQueue;


// struct kevent.flags)
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
// IMPLEMENT_ENUM(TEventFlag)

// struct kevent.filter
DEFINE_ENUM(
    TEventFilter,
    READ        = EVFILT_READ,
    WRITE       = EVFILT_WRITE,
    AIO         = EVFILT_AIO,
    VNODE       = EVFILT_VNODE,
    PROC        = EVFILT_PROC,
    SIGNAL      = EVFILT_SIGNAL,
    TIMER       = EVFILT_TIMER,
    FS          = EVFILT_FS,
    USER        = EVFILT_USER,
    SYSCOUNT    = EVFILT_SYSCOUNT
)
// IMPLEMENT_ENUM(TEventFilter)

// EVFILT_PROC
DEFINE_ENUM(
    TEventProcFFlag,
    EXIT        = NOTE_EXIT,
    FORK        = NOTE_FORK,
    EXEC        = NOTE_EXEC,
    TRACK       = NOTE_TRACK,
    TRACKERR    = NOTE_TRACKERR
)
// IMPLEMENT_ENUM(TEventProcFFlag)

// EVFILT_VNODE
DEFINE_ENUM(
    TEventVNodeFFlag,
    DELETE      = NOTE_DELETE,
    WRITE       = NOTE_WRITE,
    EXTEND      = NOTE_EXTEND,
    ATTRIB      = NOTE_ATTRIB,
    LINK        = NOTE_LINK,
    RENAME      = NOTE_RENAME,
    REVOKE      = NOTE_REVOKE
)
// IMPLEMENT_ENUM(TEventVNodeFFlag)

// EVFILT_USER
DEFINE_ENUM(
    TEventUserFFlag,
    TRIGGER     = NOTE_TRIGGER,
    FFNOP       = NOTE_FFNOP,
    FFAND       = NOTE_FFAND,
    FFOR        = NOTE_FFOR,
    FFCOPY      = NOTE_FFCOPY,
    // FFCTRLMASK  = NOTE_FFCTRLMASK,
    FFLAGSMASK  = NOTE_FFLAGSMASK
)
// IMPLEMENT_ENUM(TEventUserFFlag)


std::string toString(struct kevent const &source) {
    std::string fflags;

    // enum fflags depends filter
    switch (source.filter) {
        case EVFILT_PROC:
            // fflags = TEventProcFFlag(source.fflags).convertToFlagsString();
            fflags = convert<string, TEventProcFFlag>(static_cast<TEventProcFFlag::TEnumInternal>(source.fflags), true);
            break;
        case EVFILT_VNODE:
            // fflags = TEventVNodeFFlag(source.fflags).convertToFlagsString();
            fflags = convert<string, TEventVNodeFFlag>(static_cast<TEventVNodeFFlag::TEnumInternal>(source.fflags), true);
            break;
        case EVFILT_USER:
            // fflags = TEventUserFFlag(source.fflags).convertToFlagsString();
            fflags = convert<string, TEventVNodeFFlag>(static_cast<TEventVNodeFFlag::TEnumInternal>(source.fflags), true);
            break;
        default:
            fflags = convert<std::string, uint32_t>(source.fflags, 16);
            break;
    }

    return
        "{ ident: "     + convert<std::string>(source.ident)
        + ", filter: "  + convert<std::string, TEventFilter>(static_cast<TEventFilter::TEnumInternal>(source.filter))
        + ", flags: "   + TEventFlag(TEventFlag::TEnumInternal(source.flags)).convertToFlagsString()
        + ", fflags: "  + fflags
        + ", data: "    + convert<std::string,  intptr_t>(source.data)
        + ", udata: "   + convert<std::string, uintptr_t>(reinterpret_cast<uintptr_t>(source.udata)) + " }";
}


DEFINE_CONVERT(std::string, struct kevent);
IMPLEMENT_CONVERT(std::string, struct kevent, toString);


namespace iridium::io::implementation::platform {


static constexpr int DEFAULT_IDENT_WAKEUP = 65535;


void CMultiplexer::handleSignal(int signal) {
    LOGT << "signal: " << signal;
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
    m_kqueue(0)
{}


CMultiplexer::~CMultiplexer() {
    LOCK_SCOPE();
}


void CMultiplexer::initialize() {
    if (m_kqueue)
        throw std::runtime_error("initialization error: kqueue is not finalized"); // ----->

    m_kqueue = assertOK(kqueue(), "kqueue create error");

    try {
        struct kevent event;

        EV_SET(&event, DEFAULT_IDENT_WAKEUP, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, nullptr);

        assertOK(
            kevent(m_kqueue, &event, 1, nullptr, 0, nullptr),
           "kevent user registration error");

        m_is_initialized = true;
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer initialization error: " + string(e.what())); // ----->
    }
}


void CMultiplexer::finalize() {
    try {
        assertExists(m_kqueue.load(), "kqueue is not initialized");
        // LOGT << "finalization begin";
        m_is_initialized = false;
#ifdef FREEBSD_PLATFORM
        if (m_poll_multiplexer)
            m_poll_multiplexer->finalize();
#endif // FREEBSD_PLATFORM
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer finalization error: ") + e.what());
    }
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    if (!m_kqueue)
        return finalizeAllEvents();

    std::list<Event::TSharedPtr> events;

#ifdef FREEBSD_PLATFORM
    if (m_poll_multiplexer) {
        events = m_poll_multiplexer->waitEvents();
        if (!events.empty()) {
            // LOGT << "wake by poll ...";
            wakeKEvent();
            // LOGT << "wake by poll OK";
        }
    }
    // LOGT << "POLL EVENTS: " << events;
#endif // FREEBSD_PLATFORM

    // LOGT << "wait kevent ...";
    auto triggered_event_count = assertOK(
        kevent(
            m_kqueue,
            nullptr, 0,
            m_triggered_events.data(),
            std::min(m_triggered_events.size(),
            static_cast<size_t>(std::numeric_limits<int>::max())),
           &m_timeout),
       "kevent waiting event error");

    // LOGT << "wait kevent OK, triggered_event_count: " << triggered_event_count;

    LOCK_SCOPE();

    std::unordered_set<int> closed_process_idents;

    for (int i = 0; i < triggered_event_count; i++) {
        auto const &triggered_event = m_triggered_events[i];

        // LOGT << triggered_event;

        if (triggered_event.ident   == DEFAULT_IDENT_WAKEUP &&
            triggered_event.filter  == EVFILT_USER)
        {
            if (!m_is_initialized) {
                close(m_kqueue);
                m_kqueue = 0;

#ifdef FREEBSD_PLATFORM
                // if (m_poll_multiplexer)
                //     m_poll_multiplexer.reset();
#endif // FREEBSD_PLATFORM

                return finalizeAllEvents(); // ----->
            }

            std::vector<struct kevent>
                monitored;

            for (auto const &stream_to_handle : m_streams_to_handle->pop(false)) {
                auto const &stream      = stream_to_handle.stream;
                auto operation          = stream_to_handle.is_add_action ? Event::TOperation::OPEN : Event::TOperation::CLOSE;
                auto action             = stream_to_handle.is_add_action ? EV_ADD | EV_CLEAR : EV_DELETE;
                auto map_type_handle    = stream->getHandles();

                if (int pid = map_type_handle[IStream::THandleType::PID]) {
                    if (stream_to_handle.is_add_action)
                        m_map_pid_stream[pid] = stream;
                    else
                        m_map_pid_stream.erase(pid);

                    struct kevent e;
                    EV_SET(&e, pid, EVFILT_PROC, action, NOTE_EXIT, 0, nullptr);
                    monitored.push_back(e);
                }

                std::list< std::pair<int, short> > fd_mask_items;

                if (auto fd = map_type_handle[IStream::THandleType::READER]) {
                    fd_mask_items.push_back(
                        { fd,  EVFILT_READ }
                    );
                }

                if (auto fd = map_type_handle[IStream::THandleType::WRITER]) {
                    fd_mask_items.push_back(
                        { fd,  EVFILT_WRITE }
                    );
                }

                for (auto const &fd_mask: fd_mask_items) {
                    if (stream_to_handle.is_add_action) {
                        struct kevent e;
                        EV_SET(&e, fd_mask.first, fd_mask.second, action, 0, 0, nullptr);
                        monitored.push_back(e);
                        m_map_fd_stream.emplace(fd_mask.first, stream);
                    } else {
                        m_map_fd_stream.erase(fd_mask.first);
                    }
                }

                events.push_back(Event::create(stream, operation, Event::TStatus::END));
            }

            if (monitored.empty())
                continue; // <---

            int result = kevent(m_kqueue, monitored.data(), static_cast<int>(monitored.size()), nullptr, 0, nullptr);
            if (result < 0 && !checkOneOf(errno, ENOENT, ESRCH, EINTR, EPIPE)) {
                LOGE << "kevent update monitored events error: " << string(strerror(errno)) << ", monitored events: " << monitored;
                throw std::runtime_error(
                    "kevent update monitored events error: " + string(strerror(errno)));
            }
        } else {
            auto ident_stream  = m_map_fd_stream.find(triggered_event.ident);
            if  (ident_stream == m_map_fd_stream.end()) {
                // LOGT << "multiplexer skipping event for unmapped ident: "
                //      << convert<string>(triggered_event.ident);
                continue; // <---
            }

            auto const &stream = ident_stream->second;

            if (triggered_event.filter == EVFILT_READ)
                events.push_back(Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));

            if (triggered_event.filter == EVFILT_WRITE)
                events.push_back(Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));

            if (triggered_event.flags & EV_ERROR) {
                events.push_back(Event::create(stream, Event::TOperation::ERROR_, Event::TStatus::END));
                if (auto pid = stream->getHandles()[IStream::THandleType::PID])
                    closed_process_idents.insert(pid);
            }

            if ((triggered_event.flags   & EV_EOF) ||
                (triggered_event.filter == EVFILT_PROC &&
                (triggered_event.fflags  & NOTE_EXIT)))
            {
                events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
                if (auto pid = stream->getHandles()[IStream::THandleType::PID])
                    closed_process_idents.insert(pid);
            }
        }
    }

    events.splice(events.end(), m_wake_events->pop(false));

    return events; // ----->
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    if (!stream || stream->getHandles().empty() || !m_kqueue.load())
        return; // ----->

    try {
#ifdef FREEBSD_PLATFORM
        if (stream->getHandles()[IStream::THandleType::PID]) {
            {
                LOCK_SCOPE();
                if(!m_poll_multiplexer) {
                    m_poll_multiplexer = unix_::CMultiplexer::create();
                    m_poll_multiplexer->initialize();
                }
            }
            // LOGT << "subscribe poll ...";
            if (m_poll_multiplexer)
                m_poll_multiplexer->subscribe(stream);
            // LOGT << "subscribe poll OK";
            return; // ----->
        }
#endif // FREEBSD_PLATFORM
        m_streams_to_handle->push(TStreamToHandle { stream, true } );
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer subscribing error: ") + e.what());
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!stream || stream->getHandles().empty() || !m_kqueue.load())
        return; // ----->

    try {
#ifdef FREEBSD_PLATFORM
        if (stream->getHandles()[IStream::THandleType::PID] && m_poll_multiplexer) {
            // LOGT << "unsubscribe poll ...";
            m_poll_multiplexer->unsubscribe(stream);
            // LOGT << "unsubscribe poll OK";
            return; // ----->
        }
#endif // FREEBSD_PLATFORM
        m_streams_to_handle->push(TStreamToHandle { stream, false } );
        wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer unsubscribing error: ") + e.what());
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    try {
        m_wake_events->push(event);
        if (m_kqueue.load())
            wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer waking error: ") + e.what());
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    try {
        m_wake_events->push(events);
        if (m_kqueue.load())
            wakeKEvent();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer waking error: ") + e.what());
    }
}


void CMultiplexer::wakeKEvent() {
    struct kevent trigger;

    EV_SET(&trigger, DEFAULT_IDENT_WAKEUP, EVFILT_USER, 0, NOTE_TRIGGER, 0, nullptr);

    kevent(m_kqueue, &trigger, 1, nullptr, 0, nullptr);
}


} // iridium::io::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM
