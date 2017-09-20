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
namespace unix {


class CSocket: public ISocket {
public:
    DEFINE_CREATE(CSocket)

    CSocket(URL const &url);
    virtual ~CSocket();

    void open() override;
    void close() override;
    void write(TPacket const &packet) override;
    TPacket read() override;
    void listen() override;
    TSocketStreams accept() override;
    void interrupt() override;
    void connect() override;
    URL  getURL() const override;

protected:
    CSocket(int const &socket);
    int assertOK(int const &result, std::string const &message) const;
    URL getPeerURL(int const &socket);
    void setBlockingMode(bool const &is_blocking);

    int m_socket;
    URL m_url;
};


} // unix
} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // UNIX_PLATFORM
