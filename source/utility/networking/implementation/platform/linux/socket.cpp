#include "utility/platform.h"



#ifdef LINUX_PLATFORM


#include "socket.h"

#include "unistd.h"
#include <sys/socket.h>

#include <utility/logging/logger.h>


int const MAX_EVENTS            = 1024;
int const EPOLL_WAIT_TIMEOUT_MS = 1000;


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url)
:
    unix::CSocket   (url),
    m_epoll         (0),
    m_event         ({0}),
    m_events        (MAX_EVENTS, {0})
{}


CSocket::CSocket(int const &socket)
:
    unix::CSocket   (socket),
    m_epoll         (0),
    m_event         ({0}),
    m_events        (MAX_EVENTS, {0})
{}


void CSocket::listen() {
    unix::CSocket::listen();
    setBlockingMode(false);

    m_epoll         = assertOK(epoll_create1(0), "socket epoll create error");

    m_event.data.fd = m_socket;
    m_event.events  = EPOLLIN | EPOLLET;

    assertOK(epoll_ctl(m_epoll, EPOLL_CTL_ADD, m_socket, &m_event), "socket epoll_ctl error");
}


CSocket::TSocketStreams CSocket::accept() {
    CSocket::TSocketStreams sockets;

    auto n = epoll_wait(m_epoll, m_events.data(), MAX_EVENTS, EPOLL_WAIT_TIMEOUT_MS);
//    LOGT << "epoll_wait return count " << n;
    for (int i = 0; i < n; i++) {
        if ((m_events[i].events  & (EPOLLERR | EPOLLHUP)) ||
          ((!m_events[i].events) &  EPOLLIN))
        {
//            LOGT << "close socket " << m_events[i].data.fd;
            ::close(m_events[i].data.fd);
            continue;
        } else
        if (m_events[i].data.fd == m_socket) {
//            LOGT << "event on server socket";
            struct sockaddr     in_addr         = { 0 };
            socklen_t           in_len          = sizeof(in_addr);
            int                 client_socket   = 0;
            while (true) {
                client_socket = ::accept(m_socket, &in_addr, &in_len);
                if (client_socket == -1) {
                    if ((errno == EAGAIN) ||
                         errno == EWOULDBLOCK)
                    {
//                        LOGT << "EAGAIN | EWOULDBLOCK";
                    } else {
//                        LOGT << "accept error";
                    }
                    break;
                } else {
//                    LOGT << "acepted socket " << client_socket;
                    auto client_socket_stream = new CSocket(client_socket);
//                    client_socket_stream->setBlockingMode(false);
                    sockets.push_back(ISocketStream::TSharedPtr(client_socket_stream));
//                    LOGT << "push_back socket " << client_socket;
                }
            }
        }
    }

    return sockets;
}


} // platform
} // implementation
} // networking
} // utility


#endif // LINUX_PLATFORM
