#include "socket_acceptor.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketAcceptor::CSocketAcceptor(URL const &url)
:
    unix_::CSocketBase(url)
{}


void CSocketAcceptor::initialize() {
}


void CSocketAcceptor::finalize() {
}


URL CSocketAcceptor::getURL() const {
    return *m_url;
}


int CSocketAcceptor::getID() const {
    return 0;
}


ISocketStream::TSharedPtr CSocketAcceptor::accept() {
    return {};
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM
