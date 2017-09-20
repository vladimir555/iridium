#ifndef HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB
#define HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB


#include "utility/smart_ptr.h"
#include "utility/networking/socket.h"
#include "utility/networking/client/socket.h"
#include "utility/networking/url.h"


namespace utility {
namespace networking {
namespace client {
namespace implementation {


class CSocket: public ISocket {
public:
    DEFINE_CREATE(CSocket)
    CSocket(URL const &url);
    virtual ~CSocket() = default;

    void initialize() override;
    void finalize()   override;

    void    write(TPacket const &packet) override;
    TPacket read() override;
    void    close() override;
    URL     getURL() const override;

private:
    networking::ISocket::TSharedPtr m_socket;
};


} // implementation
} // client
} // networking
} // utility


#endif // HEADER_SOCKET_864C1C65_1CBF_4D98_8489_0E9A567DD1FB
