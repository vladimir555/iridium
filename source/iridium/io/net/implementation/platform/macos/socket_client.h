// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26
#define HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


//#include "../unix/socket_client.h"

#include "iridium/io/net/socket.h"

#include "socket.h"



namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


//typedef unix::CSocketClient CSocketClient;


class CSocketClient:
    virtual public unix_::CSocketBase,
    public ISocketStream,
    public std::enable_shared_from_this<CSocketClient>
{
public:
    DEFINE_IMPLEMENTATION(CSocketClient)
    CSocketClient(URI const &uri);

    void initialize() override;
    void finalize() override;

    URI::TSharedPtr getURI() const override;
//    int getID() const override;
    Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;

private:
//    bool assertSSLInitialized();
//    encryption::OpenSSL::TContext  *m_context;
//    encryption::OpenSSL::TSSL      *m_ssl;
//    bool                            m_is_ssl_handshake_done;
};



} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26
