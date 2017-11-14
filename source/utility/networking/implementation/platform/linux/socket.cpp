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


CSocket::CSocket(int const &socket, URL const &url, unix::CSocket::TSharedPtr const &acceptor)
:
    unix::CSocket   (socket, url, acceptor),
    m_epoll         (0),
    m_event         ({0}),
    m_events        (MAX_EVENTS, {0})
{}


void CSocket::listen() {
    unix::CSocket::listen();
    setBlockingMode(false);

    m_epoll         = assertOK(epoll_create1(0), "socket epoll create error");

    m_event.data.fd = m_socket_fd;
    m_event.events  = EPOLLIN | EPOLLET;

    assertOK(epoll_ctl(m_epoll, EPOLL_CTL_ADD, m_socket_fd, &m_event), "socket epoll_ctl error");
}


CSocket::TSocketStreams CSocket::accept() {
    CSocket::TSocketStreams sockets;

    auto n = epoll_wait(m_epoll, m_events.data(), MAX_EVENTS, EPOLL_WAIT_TIMEOUT_MS);

    for (int i = 0; i < n; i++) {
        if ((m_events[i].events  & (EPOLLERR | EPOLLHUP)) ||
          ((!m_events[i].events) &  EPOLLIN))
        {
            ::close(m_events[i].data.fd);
            continue;
        } else
        if (m_events[i].data.fd == m_socket_fd) {
            for (auto const &socket: unix::CSocket::acceptInternal()) {
                try {
                    auto client_socket_stream = new CSocket(socket, getPeerURL(socket), shared_from_this());
                    client_socket_stream->setBlockingMode(false);
                    sockets.push_back(ISocketStream::TSharedPtr(client_socket_stream));
                } catch (std::exception const &e) {
                    LOGF << e.what();
                    ::close(socket);
                }
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
#endif // LINUX_PLATFORM
