#ifndef HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4
#define HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4


#include "utility/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/socket.h"

#include <sys/epoll.h>
#include <vector>


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


class CSocket: public unix::CSocket {
public:
    DEFINE_CREATE(CSocket)
    CSocket(URL const &url);
    virtual ~CSocket() = default;

    void listen() override;
    TSocketStreams accept() override;

private:
    CSocket(int const &socket, URL const &url, unix::CSocket::TSharedPtr const &acceptor);

    int                 m_epoll;
    struct epoll_event  m_event;
    std::vector<struct epoll_event> m_events;
};


} // platform
} // implementation
} // networking
} // utility


#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4
