#include "utility/platform.h"



#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)


#ifndef HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12
#define HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#include "utility/networking/socket.h"
#include "utility/networking/url.h"


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


class CSocket: public ISocket {
public:
    DEFINE_CREATE(CSocket)
    ///
    CSocket(URL const &url);
    ///
    CSocket(URL const &url, int const &socket);
    ///
    virtual ~CSocket();
    ///
    void open() override;
    ///
    void close() override;
    ///
    void write(packet_t const &packet) override;
    ///
    packet_t read() override;
    ///
    void listen() override;
    ///
    ISocketStream::TSharedPtr accept() override;
    ///
    void interrupt() override;
    ///
    void connect() override;

private:
    ///
    URL m_url;
    ///
    int m_socket;
    ///
    int m_socket_listen_queue_size;
};


} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // LINUX_PLATFORM FREEBSD_PLATFORM MACOS_PLATFORM
