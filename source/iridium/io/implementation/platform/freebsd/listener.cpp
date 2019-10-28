#include "listener.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/logging/logger.h"
#include "iridium/convertion/convert.h"
#include "iridium/assert.h"

#include <signal.h>
#include <errno.h>
#include <string>


using std::string;
using iridium::convertion::convert;


DEFINE_ENUM(
    TEventFlag,
    // input flags
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
    // output flags
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


size_t const MAX_EVENT_COUNT    = 4;
int    const KEVENT_TIMIOUT_MS  = 10000;


//std::vector<struct kevent>  m_events        (MAX_EVENT_COUNT - 1, { 0 });
//std::vector<struct kevent>  m_monitor_events(m_events.size() + 1, { 0 });
//int                         m_monitor_events_used_count = 0;
//int                         m_kqueue;
//int                         m_socket_fd = 0;


static const struct kevent DEFAULT_KEVENT = {};


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


CListener::CListener()
:
    m_events                      (MAX_EVENT_COUNT, DEFAULT_KEVENT),
    m_monitor_events              (m_events.size(), DEFAULT_KEVENT),
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


string convertToString(std::vector<struct kevent> const &m, std::vector<struct kevent> const &e) {
    string monitor, events;
    for (auto const &i: m)
        monitor +=
        convert<string>(i.ident) + " " +
        convert<string>(i.flags) + ", ";

    for (auto const &i: e)
        events  +=
        convert<string>(i.ident) + " " +
        convert<string>(i.flags) + ", ";

    return
        "m: " + monitor + " | " +
        "e: " + events;
}

    
void CListener::add(IStream::TSharedPtr const &stream) {
    auto id = stream->getID();
    LOGT << "stream " << id;

    if (m_monitor_events_used_count < static_cast<int>(m_monitor_events.size())) {
        m_monitor_events[m_monitor_events_used_count] = {
            static_cast<uintptr_t>(id),
            TEvenFilterFlag::READ  |
            TEvenFilterFlag::WRITE, //|
            //TEvenFilterFlag::SIGNAL,
            TEventFlag::ADD     |
            TEventFlag::ONESHOT,
            0, 0, nullptr
        };
        m_monitor_events_used_count++;
        m_map_fd_stream[id] = stream;
    } else
        throw std::runtime_error("kevents buffer is full"); // ----->
    
    LOGT << "3 " << convertToString(m_monitor_events, m_events);
}


void CListener::del(IStream::TSharedPtr const &stream) {
    LOGT << "stream " << stream->getID();
}


CListener::TEvents CListener::wait() {
    static struct timespec const timeout = {
        KEVENT_TIMIOUT_MS / 1000,
        KEVENT_TIMIOUT_MS % 1000
    };

    LOGT << "1 " << convertToString(m_monitor_events, m_events);

    auto events_count = assertOK(
         kevent(m_kqueue,
             m_monitor_events.data(),
             m_monitor_events_used_count,
             m_events.data(),
             m_events.size() & std::numeric_limits<int>::max(),
             &timeout),
         "kevent waiting event error");
//    m_monitor_events_used_count = 1;

    LOGT << "2 " << convertToString(m_monitor_events, m_events);

    LOGT << "m_monitor_events_used_count " << m_monitor_events_used_count << " events_count " << events_count;
    TEvents events;
    for (int i = 0; i < events_count; i++) {
        auto &monitor_entry = m_events[i];
        if (monitor_entry.ident == 0)
            continue;
        auto stream = m_map_fd_stream[monitor_entry.ident];
        if  (stream) {
            auto event  = Event::TType::UNKNOWN;

            LOGT << "flags : " << TEventFlag        (monitor_entry.flags ).convertToFlagsString();
            LOGT << "fflags: " << monitor_entry.fflags;
            LOGT << "filter: " << TEvenFilterFlag   (monitor_entry.filter).convertToFlagsString();
//            NOTE_RENAME
            
            if (monitor_entry.flags & TEventFlag::EOF_)
                event   = Event::TType::CLOSE;

            if (monitor_entry.flags & TEventFlag::ERROR)
                event   = Event::TType::ERROR;

            if (monitor_entry.filter & TEvenFilterFlag::READ)
                event   = Event::TType::READ;

            if (monitor_entry.filter & TEvenFilterFlag::WRITE)
                event   = Event::TType::WRITE;

            LOGT << "event " << event << " reader " << stream->getID() << " ident " << monitor_entry.ident;

//            m_monitor_events[i].flags |= TEventFlag::CLEAR;

            events.push_back(Event::create(event, stream));
        } else {
            throw std::runtime_error("kevent not mapped event id " +
                convert<string>(monitor_entry.ident) +
                " flags "  + TEventFlag     (monitor_entry.flags) .convertToFlagsString() +
                " filter " + TEvenFilterFlag(monitor_entry.filter).convertToFlagsString()); // ----->
        }
    }

    return events; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
