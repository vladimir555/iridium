#include "socket_acceptor.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketAcceptor::CSocketAcceptor(URI const &uri)
:
    unix_::CSocketBase(uri)
{}


void CSocketAcceptor::initialize() {
}


void CSocketAcceptor::finalize() {
}


URI::TSharedPtr CSocketAcceptor::getURI() const {
    return m_uri;
}


//int CSocketAcceptor::getID() const {
//    return 0;
//}


ISocketStream::TSharedPtr CSocketAcceptor::accept() {
    return {};
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM
