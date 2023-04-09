// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061
#define HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061


#include "iridium/platform.h"


#if defined(LINUX_PLATFORM) || defined(FREEBSD_LIKE_PLATFORM)


#include "iridium/io/net/socket.h"
#include "iridium/encryption/openssl.h"

#include "socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix_ {


class CSocketClient:
    virtual public CSocketBase,
    public ISocketStream,
    public std::enable_shared_from_this<CSocketClient>
{
public:
    DEFINE_IMPLEMENTATION(CSocketClient)
    CSocketClient(URL const &url, IListener::TSharedPtr const &listener = nullptr);

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


#endif // LINUX_PLATFORM || FREEBSD_PLATFORM


#endif // HEADER_SOCKET_CLIENT_DC6913D5_8C89_4528_9946_093DB16A8061
