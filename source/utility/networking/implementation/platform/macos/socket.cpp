#include "utility/platform.h"


#ifdef MACOS_PLATFORM


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


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url)
:
    unix::CSocket               (url),
    m_events                    (10, { 0 }),
    m_monitor_events            (m_events.size() + 1, { 0 }),
    m_monitor_events_used_count (0)
{}


CSocket::CSocket(URL const &url, int const &socket)
:
    unix::CSocket               (url, socket),
    m_events                    (10, { 0 }),
    m_monitor_events            (m_events.size() + 1, { 0 }),
    m_monitor_events_used_count (0)
{}


void CSocket::listen() {
    assertOK( fcntl( m_socket, F_SETFL, fcntl( m_socket, F_GETFL, 0 ) | O_NONBLOCK ),
        "socket set non blocking fail" );

    unix::CSocket::listen();

    LOGT << "kqueue()";
    m_kqueue = assertOK(kqueue(), "socket kqueue create error");

    LOGT << "EV_SET";
    m_monitor_events[0] = { static_cast<uintptr_t>(m_socket), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr };
    m_monitor_events_used_count = 1;
}


CSocket::TSocketStreams CSocket::accept() {
    CSocket::TSocketStreams sockets;

    LOGT << "kevent, changes count " << m_monitor_events_used_count << " events count " << m_events.size();

    assertOK( fcntl( m_socket, F_SETFL, fcntl( m_socket, F_GETFL, 0 ) | O_NONBLOCK ),
        "socket set non blocking fail" );

    auto events_count = assertOK(
        kevent(m_kqueue, m_monitor_events.data(), m_monitor_events_used_count, m_events.data(), m_events.size(), NULL),
        "socket resolving kevent error");

//    auto events_count = assertOK(
//        kevent(m_kqueue, NULL, 0, m_events.data(), m_events.size(), NULL),
//        "socket resolving kevent error");

    m_monitor_events_used_count = 1;

    LOGT << "events_count = " << events_count;
    LOGT << "-----events:";
    for (int i = 0; i < events_count; i++) {
        string flags;
        for (auto const e: EventFlags::getEnums())
            if (static_cast<int>(e) & m_events[i].flags)
                flags += convert<string>(e) + " ";
        LOGT << "    event socket " << m_events[i].ident << " flags 0x" <<
            convert<string>(m_events[i].flags, 16) << ": " << flags;
    }
    LOGT << "-----";

    for (int i = 0; i < events_count; i++) {
        string flags;
        for (auto const e: EventFlags::getEnums())
            if (static_cast<int>(e) & m_events[i].flags)
                flags += convert<string>(e) + " ";
        LOGT << "event socket " << m_events[i].ident << " event_flags 0x" <<
            convert<string>(m_events[i].flags, 16) <<
            ": " << flags;

        if (m_events[i].ident == 0)
            continue;

        if (m_events[i].flags & EV_EOF) {
            LOGT << "EV_SET EOF delete socket " << m_events[i].ident;
            m_monitor_events[m_monitor_events_used_count] = { m_events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr };
            m_monitor_events_used_count++;
            continue;
        }

        if (m_events[i].flags & EV_ERROR) {
            auto error = std::string(strerror(m_events[i].data));
            LOGT << "event socket " << m_events[i].ident << " error: " << error;
            throw std::runtime_error("socket " + convert<string>(m_events[i].ident) + " " + error);
        }

        if (m_events[i].ident == m_socket) {
            LOGT << "accepting, socket " << m_socket;
            struct sockaddr address     = { 0 };
            socklen_t address_length    = 0;

            auto client_socket_id = assertOK(::accept(m_socket, &address, &address_length),
                "socket accept error");
            LOGT << "accepted socket " << client_socket_id;

            m_monitor_events[m_monitor_events_used_count] =
                { static_cast<uintptr_t>(client_socket_id), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr };
            m_monitor_events_used_count++;

        } else {
            LOGT << "push back for read socket " << m_events[i].ident;
            sockets.push_back(ISocketStream::TSharedPtr(
                new CSocket::CSocket(getPeerURL(m_events[i].ident), m_events[i].ident)));
        }
    }

    return sockets;
}


} // platform
} // implementation
} // networking
} // utility


#endif // MACOS_PLATFORM
