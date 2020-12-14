#ifndef HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061
#define HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061


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


class CSocketClient: public CSocketBase, public ISocketStream {
public:
    DEFINE_IMPLEMENTATION(CSocketClient)
    CSocketClient(URL const &url);

    void initialize() override;
    void finalize() override;

    URL getURL() const override;
    int getID() const override;
    Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;


private:
    bool assertSSLInitialized();

    encryption::OpenSSL::TContext  *m_context;
    encryption::OpenSSL::TSSL      *m_ssl;
    bool                            m_is_ssl_handshake_done;

};


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061
