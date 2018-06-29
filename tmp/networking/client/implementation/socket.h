#ifndef HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB
#define HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB


#include "utility/smart_ptr.h"
#include "utility/net/socket.h"
#include "utility/net/client/socket.h"
#include "utility/net/url.h"


namespace utility {
namespace net {
namespace client {
namespace implementation {


class CSocket: public ISocket {
public:
    DEFINE_CREATE(CSocket)
    CSocket(URL const &url);
    virtual ~CSocket() = default;

    void initialize() override;
    void finalize()   override;

    size_t  write(TPacket const &packet) override;
    TPacket read() override;
    void    close() override;
    URL     getURL() const override;

private:
    net::ISocket::TSharedPtr m_socket;
};


} // implementation
} // client
} // net
} // utility


#endif // HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB
