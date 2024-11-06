#include "multiplexer.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/logging/logger.h"
#include "iridium/io/implementation/event.h"
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


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


void CMultiplexer::handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
};


CMultiplexer::CMultiplexer(std::chrono::microseconds const &timeout)
:
    m_timeout {
        duration_cast<seconds>(timeout).count(),
        duration_cast<nanoseconds>(timeout).count() %
        duration_cast<nanoseconds>(
        duration_cast<seconds>(timeout)).count()
    },

    m_triggered_events( DEFAULT_EVENTS_LIMIT, (struct kevent) {0} ),

    m_kqueue    (0),
    m_pipe_add  {0},
    m_pipe_del  {0}
{}


std::array<int, 2> CMultiplexer::registerPipe() {
    std::array<int, 2> result;

    assertOK(pipe(result.data()), "pipe creating error");

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


void CMultiplexer::initialize() {
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
//        LOGT << "del pipe: " << m_pipe_del[0] << ", add pipe: " << m_pipe_add[0];
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer initialization error: " + string(e.what())); // ----->
    }
}


void CMultiplexer::finalize() {
    if (!m_kqueue)
        throw std::runtime_error("multiplexer finalization error: not initialized"); // ----->

    int64_t fd      = -1;
    auto    result  = write(m_pipe_add[1], &fd, 8);

    if (result < 0)
        throw std::runtime_error("multiplexer finalization error: " + string(std::strerror(errno))); // ----->
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    if (!m_kqueue)
        throw std::runtime_error("multiplexer subscribing error: kqueue is not initialized"); // ----->

    if (!stream || stream->getHandles().empty())
        return; // ----->

    for (auto const &fd: stream->getHandles()) {
        if (fd <= 0)
            return;
        
        {
            LOCK_SCOPE();
            m_map_fd_stream[fd] = stream;
            //        LOGT << "!   map fd: " << fd;
        }
        
        LOGT << __FUNCTION__ << ", fd: " << fd;
        // todo: optimize uintptr_t
        auto result = write(m_pipe_add[1], &fd, 8);
        
        if (result < 0)
            throw std::runtime_error("multiplexer subscribing error: " + string(strerror(errno))); // ----->
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    if (!m_kqueue)
        throw std::runtime_error("multiplexer unsubscribing error: kqueue is not initialized"); // ----->

    if (!stream || stream->getHandles().empty()) {
//        LOGW << "unsubscribe stream with null id";
        return; // ----->
    }

    for (auto const &fd: stream->getHandles()) {
        if (fd <= 0)
            return; // ----->
        
        LOGT << __FUNCTION__ << ", fd: " << fd;
        auto result = write(m_pipe_del[1], &fd, 8);
        
        if (result < 0)
            throw std::runtime_error("multiplexer subscribing error: " + string(strerror(errno))); // ----->
    }
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    if (!m_kqueue)
        return {};

    std::list<Event::TSharedPtr> events;

    auto triggered_event_count = assertOK(
         kevent(m_kqueue,
             nullptr, 0,
             m_triggered_events.data(),
             m_triggered_events.size() & std::numeric_limits<int>::max(),
            &m_timeout),
         "kevent waiting event error");

//    LOGT << "triggered_event_count: " << triggered_event_count;
    
    LOCK_SCOPE();

    bool is_finalized = false;
    for (int i = 0; i < triggered_event_count; i++) {
        auto const &triggered_event = m_triggered_events[i];

//        LOGT << __FUNCTION__ << ", id: " << triggered_event.ident << ", flags: "
//             << TEventFlag(triggered_event.flags).convertToFlagsString();
//             << ", flags " << TEventFlag(triggered_event.flags).convertToFlagsString()
//             << ", filter " << (int16_t)triggered_event.filter
//             << ", filter " << TEventFilter(triggered_event.filter)
//             << ", data " << triggered_event.data
//             << ", fflags " << triggered_event.fflags
//             << ", udata " << (uint64_t)triggered_event.udata;

        uint16_t flags = 0;
        if (triggered_event.ident == m_pipe_add[0])
            flags = EV_ADD | EV_CLEAR | EV_EOF | EV_ERROR;// | EV_ONESHOT;

        if (triggered_event.ident == m_pipe_del[0])
            flags = EV_DELETE | EV_DISABLE;

        // if flags are set then update fds and continue
        if (flags/* && triggered_event.filter & EVFILT_WRITE*/) {
            auto    byte_count  = triggered_event.data;
//            LOGT << "byte_count: " << byte_count;
            size_t  fd_count    = byte_count / 8;
//            LOGT << "fd_count: " << fd_count;

            std::vector<int64_t>        fds         (fd_count, 0);
            std::vector<struct kevent>  monitored;//   {fd_count, (struct kevent) {0}};

            auto result = read(static_cast<int>(triggered_event.ident), fds.data(), byte_count);

            if (result < 0)
                throw std::runtime_error("read control pipe error: " + string(strerror(errno)));

            for (size_t i = 0; i < fds.size(); i++) {
                auto const fd = fds[i];

                if (fd == -1) {
                    is_finalized = true;
                    break; // --->
                }

//                LOGT << "kevent update: " << fd << " " << TEventFlag(flags).convertToFlagsString();

                if (flags & EV_DELETE) {
//                    LOGT << "kevent monitored remove, fd: " << fd;
                    auto stream = m_map_fd_stream[fd];
                    if  (stream) {
//                        LOGT << "kevent monitored remove map, fd: " << fd;
                        m_map_fd_stream.erase(fd);
                        events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));
                    }
                } else {
                    monitored.push_back(
                        {
                            .ident  = static_cast<uintptr_t>(fd),
                            .filter = EVFILT_READ | EVFILT_WRITE,
                            .flags  = flags,
                            .fflags = 0,
                            .data   = 0,
                            .udata  = nullptr
                        }
                    );
                    auto stream = m_map_fd_stream[fd];
                    events.push_back(Event::create(stream, Event::TOperation::OPEN, Event::TStatus::END));
                }
            }

            if (is_finalized)
                break; // --->

            if (monitored.empty())
                continue; // <---
            
            assertOK(kevent(m_kqueue, monitored.data(), static_cast<int>(monitored.size()), nullptr, 0, nullptr),
                "kevent update monitored events error");

//            for (auto const &i: monitored) {
//                if (i.flags & EV_DELETE) {
//                    LOGT << "kevent monitored remove, fd: " << i.ident;
//                    auto stream = m_map_fd_stream[i.ident];
//                    if  (stream) {
//                        LOGT << "kevent monitored remove map, fd: " << i.ident;
//                        m_map_fd_stream.erase(i.ident);
//                        events.push_back(CEvent::create(stream, IEvent::TType::CLOSE));
//                    }
//                }
//            }

//            continue; // <---
        } else {
//            LOGT << "get from map fd: " << triggered_event.ident;
            auto const stream = m_map_fd_stream[triggered_event.ident];
            if  (stream) {
                if (triggered_event.flags & EV_EOF) {
//                    LOGT << "! EOF, id: " << stream->getID();
//                    events.push_back(Event::create(stream, Event::TOperation::EOF_, Event::TStatus::END));
                    events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
                    continue; // <---
                }

                if (triggered_event.filter == EVFILT_READ)
                    events.push_back(Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));

                if (triggered_event.filter == EVFILT_WRITE)
                    events.push_back(Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));

                if (triggered_event.flags & EV_ERROR)
                    events.push_back(Event::create(stream, Event::TOperation::ERROR_));

            } else {
                // freebsd bug
                LOGW << "multiplexer waiting events error: kevent not mapped event, fd: "
                     << convert<string>(triggered_event.ident);
//                throw std::runtime_error(
//                    "multiplexer waiting events error: kevent not mapped event, fd: " +
//                    convert<string>(triggered_event.ident)); // ----->
            }
        }
    }

    if (is_finalized) {
        close(m_kqueue);
        m_kqueue = 0;
    }

    {
        std::string msg;
        for (auto const &event: events)
            msg += event->stream->getHandles().empty() ? 
                   "" : convert<std::string>(event->stream->getHandles().front())
                + " " + convert<std::string>(event->operation)
                + " " + convert<std::string>(event->status);
        LOGT << __func__ << ":\n" << msg << "\nmap size: " << m_map_fd_stream.size();
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
