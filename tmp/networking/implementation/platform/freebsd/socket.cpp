#include "utility/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "socket.h"

#include "utility/logging/logger.h"
#include "utility/assert.h"
#include "utility/enum.h"
#include "utility/encryption/implementation/ssl.h"
#include "utility/threading/synchronized_scope.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>


using utility::convertion::convert;
using std::string;


DEFINE_ENUM(
    EventFlags,
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


IMPLEMENT_ENUM(EventFlags)


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


size_t const MAX_EVENT_COUNT    = 128;
int    const KEVENT_TIMIOUT_MS  = 1000;


CSocket::CSocket(URL const &url)
:
    unix::CSocket               (url),
    m_events                    (MAX_EVENT_COUNT - 1, { 0 }),
    m_monitor_events            (m_events.size() + 1, { 0 }),
    m_monitor_events_used_count (0)
{}


void CSocket::listen() {
    setBlockingMode(false);
    unix::CSocket::listen();

    m_kqueue = assertOK(kqueue(), "socket kqueue create error");
    m_monitor_events[0] = { static_cast<uintptr_t>(m_socket_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr };
    m_monitor_events_used_count = 1;
}


ISocket::TEvents CSocket::accept() {
    LOGT << "accept";
    TEvents events;

    static struct timespec const timeout = { KEVENT_TIMIOUT_MS / 1000, KEVENT_TIMIOUT_MS % 1000 };

    sleep(1);
    auto events_count = assertOK(
        kevent(m_kqueue,
               m_monitor_events.data(), m_monitor_events_used_count,
               m_events.data(), m_events.size() & std::numeric_limits<int>::max(),
               &timeout),
        "socket resolving kevent error");
    m_monitor_events_used_count = 1;

    for (int i = 0; i < events_count; i++) {
        if (m_events[i].ident == 0)
            continue; // <---

        LOGT << "fd " << m_events[i].ident << " flags " << EventFlags(m_events[i].flags).convertToFlagsString();
        LOGT << "accept fd " << m_socket_fd;

        if (m_events[i].flags & EV_EOF) {
//            LOGT << getPeerURL(m_events[i].ident) << "EOF";
            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue; // <---
            m_monitor_events[m_monitor_events_used_count] =
                { m_events[i].ident, EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, nullptr };
            m_monitor_events_used_count++;
            continue; // <---
        }

        if (m_events[i].flags & EV_ERROR) {
            LOGE <<  "kevent error: " <<
                // string(strerror(m_events[i].data)) << " " <<
                EventFlags(m_events[i].flags).convertToFlagsString() <<
                " queue size " << m_monitor_events_used_count;
            continue; // <---
        }

        if (m_events[i].ident == m_socket_fd) {
            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue; // <---

            std::list<int> accepted_sockets_fd;
            auto sse = unix::CSocket::acceptInternal(accepted_sockets_fd);
            for (auto const &socket_fd: accepted_sockets_fd) {
                m_monitor_events[m_monitor_events_used_count] =
                    { static_cast<uintptr_t>(socket_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr };
                m_monitor_events_used_count++;
            }
            events.insert(events.end(), sse.begin(), sse.end());
        } else {
            try {
                auto action = TEvent::TAction::UNKNOWN;
                
                if (m_events[i].fflags & EVFILT_READ)
                    action = TEvent::TAction::READ;
                if (m_events[i].fflags & EVFILT_WRITE)
                    action = TEvent::TAction::WRITE;

                auto socket = CSocket::create(getPeerURL(static_cast<int>(m_events[i].ident)));
                auto event  = TEvent::create(action, socket);
                
                events.push_back(event);
                
//                auto socket_event       = TEvent::create();

////                socket_event->socket = findAcceptedSocket(m_events[i].ident);

//                auto socket             = CSocket::create(getPeerURL(m_events[i].ident));
//                socket->m_socket_fd     = m_events[i].ident;
//                socket->m_encryptor     = m_encryptor;
//                socket_event->socket    = socket;

//                if (m_events[i].fflags & EVFILT_READ)
//                    socket_event->action = TEvent::TAction::READ;
//                if (m_events[i].fflags & EVFILT_WRITE)
//                    socket_event->action = TEvent::TAction::WRITE;
//                sockets_events.push_back(socket_event);

//                auto socket = unix::CSocket::createInternal(m_events[i].ident);
//                if (socket)
//                    sockets_events.push_back(ISocketStream::TSharedPtr(socket));
//                LOGT << "fd " << m_events[i].ident << " push_back " << socket->getURL() << " flags " << m_events[i].flags <<
//                    " " << EventFlags(m_events[i].flags).convertToFlagsString();
            } catch (std::exception const &e) {
                LOGF << e.what();
                ::close(m_events[i].ident);
            }
        }
    }
    LOGT << "return count " << events.size();
    return events; // ----->
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // FREEBSD_LIKE_PLATFORM
