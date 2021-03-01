#ifndef HEADER_SOCKET_ACCEPTOR_1D0E7ED6_8B6F_48B7_BEA9_1C2248714CA3
#define HEADER_SOCKET_ACCEPTOR_1D0E7ED6_8B6F_48B7_BEA9_1C2248714CA3


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/net/socket.h"
#include "iridium/io/listener.h"

#include "iridium/encryption/openssl.h"

#include "socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


class CSocketAcceptor: public CSocketBase, public ISocketAcceptor, public std::enable_shared_from_this<CSocketAcceptor> {
public:
    DEFINE_IMPLEMENTATION(CSocketAcceptor)
    CSocketAcceptor(URL const &url, IListenerStreams::TSharedPtr const &listener = nullptr);

    void initialize() override;
    void finalize() override;

    URL getURL() const override;
    int getID() const override;

    ISocketStream::TSharedPtr accept() override;

private:
    encryption::OpenSSL::TContext *m_context;
};


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_1D0E7ED6_8B6F_48B7_BEA9_1C2248714CA3
