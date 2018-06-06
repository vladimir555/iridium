#include "listener.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "utility/logging/logger.h"
#include "utility/convertion/convert.h"
#include "utility/assert.h"

#include <signal.h>
#include <errno.h>
#include <string>


using std::string;
using utility::convertion::convert;


DEFINE_ENUM(
    TEventFlag,

    ADD             = EV_ADD,
    DELETE          = EV_DELETE,
    ENABLE          = EV_ENABLE,
    DISABLE         = EV_DISABLE,
    ONESHOT         = EV_ONESHOT,
    CLEAR           = EV_CLEAR,
    RECEIPT         = EV_RECEIPT,
    DISPATCH        = EV_DISPATCH,
    UDATA_SPECIFIC  = EV_UDATA_SPECIFIC,
    DISPATCH2       = EV_DISPATCH | EV_UDATA_SPECIFIC,
    VANISHED        = EV_VANISHED,
    SYSFLAGS        = EV_SYSFLAGS,
    FLAG0           = EV_FLAG0,
    FLAG1           = EV_FLAG1,

    EOF_            = EV_EOF,
    ERROR           = EV_ERROR
)


DEFINE_ENUM(TEvenFilterFlag,
    READ            = EVFILT_READ,
    WRITE           = EVFILT_WRITE,
    AIO             = EVFILT_AIO,
    VNODE           = EVFILT_VNODE,
    PROC            = EVFILT_PROC,
    SIGNAL          = EVFILT_SIGNAL,
    TIMER           = EVFILT_TIMER,
    MACHPORT        = EVFILT_MACHPORT,
    FS              = EVFILT_FS,
    USER            = EVFILT_USER,
    VM              = EVFILT_VM,
    EXCEPT          = EVFILT_EXCEPT,
    SYSCOUNT_THREADMARKER = EVFILT_THREADMARKER
)


IMPLEMENT_ENUM(TEventFlag)
IMPLEMENT_ENUM(TEvenFilterFlag)


void handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
}


template<typename T>
T assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " +
              std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    else
        return result; // ----->
}


size_t const MAX_EVENT_COUNT    = 3;
int    const KEVENT_TIMIOUT_MS  = 1000;


std::vector<struct kevent>  m_events        (MAX_EVENT_COUNT - 1, { 0 });
std::vector<struct kevent>  m_monitor_events(m_events.size() + 1, { 0 });
int                         m_monitor_events_used_count = 0;
int                         m_kqueue;
int                         m_socket_fd = 0;


namespace utility {
namespace io {
namespace implementation {
namespace platform {


CListener::CListener()
:
    m_events                      (MAX_EVENT_COUNT - 1, { 0 }),
    m_monitor_events              (m_events.size() + 1, { 0 }),
    m_monitor_events_used_count   (0),
    m_kqueue                      (0)
{}


void CListener::initialize() {
    LOGT << "set empty signal handler for broken pipe";
    struct sigaction sh;
    struct sigaction osh;

    // can set to SIG_IGN
    sh.sa_handler   = &handleSignal;
    // restart interrupted system calls
    sh.sa_flags     = SA_RESTART;

    // block every signal during the handler
    sigemptyset(&sh.sa_mask);

    if (sigaction(SIGPIPE, &sh, &osh) < 0)
        throw std::runtime_error("sigaction error");

    m_kqueue = assertOK(kqueue(), "socket kqueue create error");
}


void CListener::finalize() {

}


void CListener::add(IStream::TSharedPtr const &stream) {
    if (m_monitor_events_used_count < m_monitor_events.size()) {
        m_monitor_events[m_monitor_events_used_count] = {
            static_cast<uintptr_t>(stream->getID()),
            TEvenFilterFlag::READ | TEvenFilterFlag::WRITE,
            TEventFlag::ADD,
            0, 0, nullptr
        };
        m_monitor_events_used_count++;
        m_map_fd_stream[stream->getID()] = stream;
    } else
        throw std::runtime_error("kevents buffer is full"); // ----->
}


void CListener::del(IStream::TSharedPtr const &stream) {

}


CListener::TEvents CListener::wait() {
    struct timespec const timeout = { KEVENT_TIMIOUT_MS / 1000, KEVENT_TIMIOUT_MS % 1000 };

    auto events_count = assertOK(
         kevent(m_kqueue,
             m_monitor_events.data(),
             m_monitor_events_used_count,
             m_events.data(),
             m_events.size() & std::numeric_limits<int>::max(),
             &timeout),
         "kevent waiting event error");

    TEvents events;
    for (int i = 0; i < events_count; i++) {
        auto const &kevent_entry = m_monitor_events[i];
        auto stream = m_map_fd_stream[kevent_entry.ident];
        if  (stream) {
            auto event  = Event::TEvent::UNKNOWN;

            if (kevent_entry.flags & TEventFlag::EOF_)
                event   = Event::TEvent::CLOSE;

            if (kevent_entry.flags & TEventFlag::ERROR)
                event   = Event::TEvent::ERROR;

            if (kevent_entry.filter & TEvenFilterFlag::READ)
                event   = Event::TEvent::READ;

            if (kevent_entry.filter & TEvenFilterFlag::WRITE)
                event   = Event::TEvent::WRITE;

            events.push_back(Event::create(event, stream));
        } else {
            throw std::runtime_error("kevent not mapped event id " +
                convert<string>(kevent_entry.ident) +
                " flags "  + TEventFlag     (kevent_entry.flags) .convertToFlagsString() +
                " filter " + TEvenFilterFlag(kevent_entry.filter).convertToFlagsString()); // ----->
        }
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // utility


#endif // FREEBSD_LIKE_PLATFORM
