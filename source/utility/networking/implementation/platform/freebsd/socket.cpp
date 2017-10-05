#include "utility/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "socket.h"

#include "utility/logging/logger.h"
#include "utility/assert.h"
#include "utility/enum.h"

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


string flagsToString(int const &flags) {
    string result;
    for (auto const &f: EventFlags::getEnums())
        if (flags & f)
            result += convert<string>(f) + " ";
    return result;
}


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


CSocket::CSocket(int const &socket)
:
    unix::CSocket               (socket),
    m_events                    (MAX_EVENT_COUNT - 1, { 0 }),
    m_monitor_events            (m_events.size() + 1, { 0 }),
    m_monitor_events_used_count (0)
{}


void CSocket::listen() {
    assertOK( fcntl( m_socket, F_SETFL, fcntl( m_socket, F_GETFL, 0 ) | O_NONBLOCK ),
        "socket set non blocking fail" );

    unix::CSocket::listen();

    m_kqueue = assertOK(kqueue(), "socket kqueue create error");

    m_monitor_events[0] = { static_cast<uintptr_t>(m_socket), EVFILT_READ, EV_ADD, 0, 0, nullptr };
    m_monitor_events_used_count = 1;

    setBlockingMode(false);
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
            continue;

        if (m_events[i].flags & EV_EOF) {
            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue;
            m_monitor_events[m_monitor_events_used_count] =
                { m_events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr };
            m_monitor_events_used_count++;
            continue;
        }

        if (m_events[i].flags & EV_ERROR) {
//            LOGE <<  CSocket(m_events[i].ident).getURL() << " kevent error: " <<
//                     string(strerror(m_events[i].data))  << " " << flagsToString(m_events[i].flags) <<
//                     " queue size " << m_monitor_events_used_count;
//            ::close(m_events[i].ident);
//            if (m_monitor_events_used_count >= m_monitor_events.size())
//                continue;
//            m_monitor_events[m_monitor_events_used_count] =
//                { m_events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr };
//            m_monitor_events_used_count++;
            continue;
        }

        if (m_events[i].ident == m_socket) {
            struct sockaddr address             = { 0 };
            socklen_t       address_length      =   0;
            int             client_socket_id    =   0;

            if (m_monitor_events_used_count >= m_monitor_events.size())
                continue;

            do {
                client_socket_id = ::accept(m_socket, &address, &address_length);
                if (client_socket_id > 0) {
                    m_monitor_events[m_monitor_events_used_count] =
                        { static_cast<uintptr_t>(client_socket_id), EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, nullptr };
                    m_monitor_events_used_count++;
                }
            } while(client_socket_id > 0);

        } else {
            auto client_socket_stream = new CSocket(m_events[i].ident);
            client_socket_stream->setBlockingMode(false);
//            LOGT << "push_back " << client_socket_stream->getURL() << " flags " << m_events[i].flags <<
//                " " << flagsToString(m_events[i].flags);
            sockets.push_back(ISocketStream::TSharedPtr(client_socket_stream));
        }
    }

    return sockets;
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // FREEBSD_LIKE_PLATFORM
