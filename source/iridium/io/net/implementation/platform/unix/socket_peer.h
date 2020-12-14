#ifndef HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D
#define HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/net/socket.h"
#include "iridium/encryption/openssl.h"

#include "socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


class CSocketPeer: public CSocketBase, public ISocketStream {
public:
    DEFINE_IMPLEMENTATION(CSocketPeer)

    CSocketPeer(int const &fd, URL::TProtocol const &protocol);
    CSocketPeer(int const &fd, URL::TProtocol const &protocol, encryption::OpenSSL::TSSL *ssl);

    void initialize() override;
    void finalize() override;

    URL getURL() const override;
    int getID() const override;
    Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;

private:
    URL::TProtocol              m_protocol;
    encryption::OpenSSL::TSSL  *m_ssl;
};


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D
