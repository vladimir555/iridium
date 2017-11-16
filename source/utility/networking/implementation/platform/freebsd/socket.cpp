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
    m_monitor_events[0] = { static_cast<uintptr_t>(m_socket_fd), EVFILT_READ, EV_ADD, 0, 0, nullptr };
    m_monitor_events_used_count = 1;
}


CSocket::TSocketStreams CSocket::accept() {
    CSocket::TSocketStreams sockets;

    static struct timespec const timeout = { KEVENT_TIMIOUT_MS / 1000, KEVENT_TIMIOUT_MS % 1000 };

    auto events_count = assertOK(
        kevent(m_kqueue, m_monitor_events.data(), m_monitor_events_used_count, m_events.data(), m_events.size(), &timeout),
        "socket resolving kevent error");
    m_monitor_events_used_count = 1;

    for (int i = 0; i < events_count; i++) {
        if (m_events[i].ident == 0)
            continue; // <---

        if (m_events[i].flags & EV_EOF) {
//            LOGT << getPeerURL(m_events[i].ident) << "EOF";
            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue; // <---
            m_monitor_events[m_monitor_events_used_count] =
                { m_events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr };
            m_monitor_events_used_count++;
            continue; // <---
        }

        if (m_events[i].flags & EV_ERROR) {
            LOGE <<  "kevent error: " <<
                string(strerror(m_events[i].data)) << " " <<
                EventFlags(m_events[i].flags).convertToFlagsString() <<
                " queue size " << m_monitor_events_used_count;
            continue; // <---
        }

        if (m_events[i].ident == m_socket_fd) {
            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue; // <---

            for (auto const &socket: unix::CSocket::acceptInternal()) {
                m_monitor_events[m_monitor_events_used_count] =
                    { static_cast<uintptr_t>(socket), EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, nullptr };
                m_monitor_events_used_count++;
            }
        } else {
            try {
                auto socket = unix::CSocket::createInternal(m_events[i].ident);
                if (socket)
                    sockets.push_back(ISocketStream::TSharedPtr(socket));

//                auto client_socket = new CSocket(m_events[i].ident, getPeerURL(m_events[i].ident), this);
//                client_socket->setBlockingMode(false);
//                CSocket::TSharedPtr client_socket_ptr(client_socket);

//                if (create(client_socket_ptr))
//                    sockets.push_back(ISocketStream::TSharedPtr(client_socket_ptr));
//                LOGT << "push_back " << client_socket_stream->getURL() << " flags " << m_events[i].flags <<
//                    " " << EventFlags(m_events[i].flags).convertToFlagsString();
            } catch (std::exception const &e) {
                LOGF << e.what();
                ::close(m_events[i].ident);
            }
        }
    }
    return sockets; // ----->
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // FREEBSD_LIKE_PLATFORM
