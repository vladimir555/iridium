#ifndef HEADER_SOCKET_553A6A20_A0BF_46B8_8CDB_9040D8804616
#define HEADER_SOCKET_553A6A20_A0BF_46B8_8CDB_9040D8804616


#include "utility/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/socket.h"

#include <sys/event.h>
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
    CSocket(URL const &url, int const &socket);

    std::vector<struct kevent>  m_events;
    std::vector<struct kevent>  m_monitor_events;
    size_t                      m_monitor_events_used_count;

    int m_kqueue;
};


} // platform
} // implementation
} // networking
} // utility


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_553A6A20_A0BF_46B8_8CDB_9040D8804616
