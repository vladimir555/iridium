#include "event_provider.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/logging/logger.h"
#include "iridium/io/implementation/event.h"
#include "iridium/threading/mutex.h"
#include "iridium/threading/synchronized_scope.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/implementation/worker.h"
#include "iridium/items.h"

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
using iridium::threading::implementation::CMutex;


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
    ERROR       = EV_ERROR
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
    MACHPORT    = EVFILT_MACHPORT,
    FS          = EVFILT_FS,
    USER        = EVFILT_USER,
    VM          = EVFILT_VM,
    EXCEPT      = EVFILT_EXCEPT,
    SYSCOUN     = EVFILT_SYSCOUNT
)


IMPLEMENT_ENUM(TEventFilter)


enum E {
    E1 = std::numeric_limits<int>::min()
};


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


void CEventProvider::handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
};


CEventProvider::CEventProvider(std::chrono::microseconds const &timeout)
:
    Synchronized(CMutex::create()),

    m_timeout {
        duration_cast<seconds>(timeout).count(),
        duration_cast<nanoseconds>(timeout).count() %
        duration_cast<nanoseconds>(
        duration_cast<seconds>(timeout)).count()
    },

    m_triggered_events( DEFAULT_EVENTS_LIMIT, (struct kevent) {0} ),

    m_kqueue        (0),
    m_pipe_add      {0},
    m_pipe_del      {0}
{}


std::array<int, 2> CEventProvider::registerPipe() {
    std::array<int, 2> result;

    assertOK(pipe(result.data()), "pipe creating error");

//    LOGT << "pipe: " << result[0] << " " << result[1];

    struct kevent event {
        .ident  = static_cast<uintptr_t>(result[0]),
        .filter = EVFILT_READ,
        .flags  = EV_ADD | EV_CLEAR,
        .fflags = 0,
        .data   = 0,
        .udata  = nullptr
    };

    assertOK(kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent pipe registration error");

    return result;
};


void CEventProvider::initialize() {
//    LOGT << __FUNCTION__;

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
        m_pipe_add = registerPipe();
        m_pipe_del = registerPipe();
    } catch (std::exception const &e) {
        throw std::runtime_error("event provider initialization error: " + string(e.what())); // ----->
    }
}


void CEventProvider::finalize() {
//    LOGT << __FUNCTION__;
    if (!m_kqueue)
        throw std::runtime_error("event profider finalization error: not initialized"); // ----->

    int64_t fd      = -1;
    auto    result  = write(m_pipe_add[1], &fd, 8);

    if (result < 0)
        throw std::runtime_error("event provider finalization error: " + string(strerror(errno))); // ----->
}


void CEventProvider::subscribe(IStream::TSharedPtr const &stream) {
    if (!m_kqueue)
        throw std::runtime_error("event provider subscribing error: kqueue is not initialized"); // ----->

    int64_t fd = stream->getID();

    if (fd <= 0)
        return;

    LOGT << __FUNCTION__ << " fd: " << fd;

    // todo: optimize uintptr_t
    auto    result  = write(m_pipe_add[1], &fd, 8);

    if (result < 0)
        throw std::runtime_error("event provider subscribing error: " + string(strerror(errno))); // ----->

    LOCK_SCOPE;
    m_map_fd_stream[fd] = stream;
}


void CEventProvider::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!m_kqueue)
        throw std::runtime_error("event provider unsubscribing error: kqueue is not initialized"); // ----->

    int64_t fd = stream->getID();
    if (fd <= 0)
        return;

    LOGT << __FUNCTION__ << " fd: " << fd;

//    auto    result  = write(m_pipe_del[1], &fd, 8);
//    LOGT << "unsubscribe write result: " << result;
//    if (result < 0)
//        throw std::runtime_error("event provider unsubscribing error: " + string(strerror(errno))); // ----->
}


std::list<IEvent::TSharedPtr> CEventProvider::waitEvents() {
    std::list<IEvent::TSharedPtr> events;

    if (m_kqueue == 0)
        return events; // ----->

//    LOGT << "!!!!! WAIT ...";
    auto triggered_event_count = assertOK(
         kevent(m_kqueue,
             nullptr, 0,
             m_triggered_events.data(),
             m_triggered_events.size() & std::numeric_limits<int>::max(),
             &m_timeout),
         "kevent waiting event error");
//    LOGT << "!!!!! WAIT OK, triggered event count: " << triggered_event_count;

    LOCK_SCOPE;
    bool is_finalized = false;
    for (int i = 0; i < triggered_event_count; i++) {
        auto const &triggered_event = m_triggered_events[i];

        LOGT << "triggered_event: ident " << triggered_event.ident
             << ", flags " << TEventFlag(triggered_event.flags).convertToFlagsString()
             << ", filter " << (int16_t)triggered_event.filter
             << ", filter " << TEventFilter(triggered_event.filter)
             << ", data " << triggered_event.data
             << ", fflags " << triggered_event.fflags
             << ", udata " << (uint64_t)triggered_event.udata;

        uint16_t flags = 0;
        if (triggered_event.ident == m_pipe_add[0])
            flags = EV_ADD | EV_ONESHOT;// | EV_EOF;

        if (triggered_event.ident == m_pipe_del[0])
            flags = EV_DELETE;

        if (flags && triggered_event.filter & EVFILT_WRITE) {
            auto    byte_count  = triggered_event.data;
            size_t  fd_count    = byte_count / 8;

//            LOGT << "pipe bytes count: " << byte_count;

            std::vector<int64_t>        fds         (fd_count, 0);
            std::vector<struct kevent>  monitored;//   {fd_count, (struct kevent) {0}};

            auto result = read(triggered_event.ident, fds.data(), byte_count);

            if (result < 0)
                throw std::runtime_error("read control pipe error: " + string(strerror(errno)));

//            LOGT << "read fds size: " << result;

            for (size_t i = 0; i < fds.size(); i++) {
                auto const &fd = fds[i];

                if (fd == -1) {
                    is_finalized = true;
                    break; // --->
                }

                if (flags == EV_DELETE)
                    m_map_fd_stream[fd] = nullptr;

//                monitored[i] = {
                monitored.push_back(
                    {
                        .ident  = static_cast<uintptr_t>(fd),
                        .filter = EVFILT_WRITE,// | EVFILT_READ,
                        .flags  = flags,
                        .fflags = 0,
                        .data   = 0,
                        .udata  = nullptr
                    }
                );

//                LOGT << "kevent update: " << fd << " " << TEventFlag(flags).convertToFlagsString();
            }

            if (is_finalized)
                break; // --->

            assertOK(kevent(m_kqueue, monitored.data(), monitored.size(), nullptr, 0, nullptr),
                "kevent update monitored events error");

            continue; // <---
        }

        auto const stream = m_map_fd_stream[triggered_event.ident];
        if  (stream) {
//            IEvent::TType event_type = IEvent::TType::UNKNOWN;

            if (triggered_event.filter & EVFILT_READ)
                events.push_back(CEvent::create(stream, IEvent::TType::READ));

            if (triggered_event.filter & EVFILT_WRITE)
                events.push_back(CEvent::create(stream, IEvent::TType::WRITE));

            if (triggered_event.flags & EV_ERROR)
                events.push_back(CEvent::create(stream, IEvent::TType::ERROR));

            if (triggered_event.flags & EV_EOF)
                events.push_back(CEvent::create(stream, IEvent::TType::CLOSE));

//            LOGT << "event: " << event_type
//                 << ", fd: " << stream->getID()
//                 << ", ident: " << triggered_event.ident;

//            events.push_back(CEvent::create(stream, event_type));
        } else {
//            LOGE << "event provider waiting error: kevent not mapped event, fd: " +
//                convert<string>(triggered_event.ident);
            throw std::runtime_error(
                "event provider waiting error: kevent not mapped event, fd: " +
                convert<string>(triggered_event.ident)); // ----->
        }
    }

    if (is_finalized) {
//        LOGT << "finalize kevent";
        close(m_kqueue);
        m_kqueue = 0;
    }

    LOGT << __FUNCTION__ << ", events:";
//    string events_str;
    for (auto const &event: events) {
        LOGT << event->getStream()->getID() << " " << event->getType();
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
