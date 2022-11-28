#ifndef HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29
#define HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "iridium/io/net/socket.h"
#include "../unix/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


//typedef unix::CSocketAcceptor CSocketAcceptor;
class CSocketAcceptor:
    public unix_::CSocketBase,
    public ISocketAcceptor,
    public std::enable_shared_from_this<CSocketAcceptor>
{
public:
    DEFINE_IMPLEMENTATION(CSocketAcceptor)
    CSocketAcceptor(URL const &url);

    void initialize() override;
    void finalize() override;

    URL getURL() const override;
    int getID() const override;

    ISocketStream::TSharedPtr accept() override;

private:
//    encryption::OpenSSL::TContext *m_context;
};


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29
