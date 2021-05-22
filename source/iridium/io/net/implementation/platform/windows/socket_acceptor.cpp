#include "socket_acceptor.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketAcceptor::CSocketAcceptor(URL const& url, IListenerStreams::TSharedPtr const& listener) {
    throw std::runtime_error("not implemented");
}


void CSocketAcceptor::initialize() {
}


void CSocketAcceptor::finalize() {
}


URL CSocketAcceptor::getURL() const {
    return URL("");
}


int CSocketAcceptor::getID() const {
    return {};
}


ISocketStream::TSharedPtr CSocketAcceptor::accept() {
    return {};
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM
