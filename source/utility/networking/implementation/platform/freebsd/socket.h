#ifndef HEADER_SOCKET_6EC11FC9_CBC3_439A_8053_B2E0D1EFC2B6
#define HEADER_SOCKET_6EC11FC9_CBC3_439A_8053_B2E0D1EFC2B6


#include "utility/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "../unix/socket.h"

#include <sys/event.h>
#include <vector>


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


//typedef unix::CSocket CSocket;


class CSocket: public unix::CSocket {
public:
    DEFINE_CREATE(CSocket)
    CSocket(URL const &url);
    virtual ~CSocket() = default;

    void listen() override;
    TEvents accept() override;
private:
    std::vector<struct kevent>  m_events;
    std::vector<struct kevent>  m_monitor_events;
    size_t                      m_monitor_events_used_count;
    int                         m_kqueue;
};


} // platform
} // implementation
} // networking
} // utility


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_SOCKET_6EC11FC9_CBC3_439A_8053_B2E0D1EFC2B6
