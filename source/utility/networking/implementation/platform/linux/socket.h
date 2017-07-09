#include "utility/platform.h"



#ifdef UNIX_PLATFORM


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

    CSocket(URL const &url, bool const &is_blocking_connection = false);
    virtual ~CSocket();

    void open() override;
    void close() override;
    void write(packet_t const &packet) override;
    packet_t read() override;
    void listen() override;
    ISocketStream::TSharedPtr accept() override;
    void interrupt() override;
    void connect() override;

private:
    CSocket(URL const &url, int const &socket);

    int assertOK(int const &result, std::string const &message) const;

    URL m_url;
    int m_socket;
    bool m_is_blocking_connection;
};


} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // UNIX_PLATFORM
