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


DEFINE_ENUM(
    EPollEvent,
        IN      = EPOLL_EVENTS::EPOLLIN,
        PRI     = EPOLL_EVENTS::EPOLLPRI,
        OUT     = EPOLL_EVENTS::EPOLLOUT,
        RDNORM  = EPOLL_EVENTS::EPOLLRDNORM,
        RDBAND  = EPOLL_EVENTS::EPOLLRDBAND,
        WRNORM  = EPOLL_EVENTS::EPOLLWRNORM,
        WRBAND  = EPOLL_EVENTS::EPOLLWRBAND,
        MSG     = EPOLL_EVENTS::EPOLLMSG,
        ERR     = EPOLL_EVENTS::EPOLLERR,
        HUP     = EPOLL_EVENTS::EPOLLHUP,
        RDHUP   = EPOLL_EVENTS::EPOLLRDHUP,
        WAKEUP  = EPOLL_EVENTS::EPOLLWAKEUP,
        ONESHOT = EPOLL_EVENTS::EPOLLONESHOT,
        ET      = EPOLL_EVENTS::EPOLLET
)


CSocket::CSocket(URL const &url)
:
    unix::CSocket   (url),
    m_epoll         (0),
    m_event         ({0}),
    m_events        (MAX_EVENTS, {0})
{}


void CSocket::listen() {
    setBlockingMode(false);

    unix::CSocket::listen();

    m_epoll         = assertOK(epoll_create1(0), "socket epoll create error");

    m_event.data.fd = m_socket_fd;
    m_event.events  = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;

    assertOK(epoll_ctl(m_epoll, EPOLL_CTL_ADD, m_socket_fd, &m_event), "socket epoll_ctl error");
}


CSocket::TEvents CSocket::accept() {
    TEvents events;

//    auto n = epoll_wait(m_epoll, m_events.data(), MAX_EVENTS, EPOLL_WAIT_TIMEOUT_MS);

//    for (int i = 0; i < n; i++) {
//        LOGT << "fd " << m_events[i].data.fd << " events " << m_events[i].events;
//        if ((m_events[i].events  & (EPOLLERR | EPOLLHUP)) ||
//          ((!m_events[i].events) &  EPOLLIN))
//        {
//            ::close(m_events[i].data.fd);
//            continue;
//        } else
//        if (m_events[i].data.fd == m_socket_fd) {
//            std::list<int> accepted_sockets_fd;
//            sockets = unix::CSocket::acceptInternal(accepted_sockets_fd);
//    }


    auto n = epoll_wait(m_epoll, m_events.data(), MAX_EVENTS, EPOLL_WAIT_TIMEOUT_MS);
    assertOK(n, "epoll waiting error");
    for (int i = 0; i < n; i++) {

        LOGT << "fd " << m_events[i].data.fd << " event "
            << EPollEvent(m_events[i].events).convertToFlagsString();

        if (m_events[i].data.fd == m_socket_fd) {
            std::list<int> accepted_sockets_fd;
            events = unix::CSocket::acceptInternal(accepted_sockets_fd);

            for (auto fd: accepted_sockets_fd) {
                struct epoll_event event;
                event.events  = EPOLLIN | EPOLLOUT;
                event.data.fd = fd;
                epoll_ctl(m_epoll, EPOLL_CTL_ADD, fd, &event);
            }
        } else {
            auto event      = TEvent::create();
            event->action   = TEvent::TAction::ACCEPT;
            //event->socket   = socket;

            events.push_back(event);
        }
    }
    sleep(1);

    return events; // ----->
}


void CSocket::close() {
    ::close(m_epoll);
    unix::CSocket::close();
}


} // platform
} // implementation
} // networking
} // utility


IMPLEMENT_ENUM(utility::networking::implementation::platform::EPollEvent)


#else
void dummy() {}
#endif // LINUX_PLATFORM
