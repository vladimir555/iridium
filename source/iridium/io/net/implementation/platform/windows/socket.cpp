#include "socket.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url, bool const &is_server_mode) {
    throw std::runtime_error("windows socket not implemented");
}


void CSocket::initialize() {
    throw std::runtime_error("windows socket not implemented");
}


void CSocket::finalize() {
    throw std::runtime_error("windows socket not implemented");
}


ISocket::TSharedPtr CSocket::accept() {
    throw std::runtime_error("windows socket not implemented");
}


URL CSocket::getURL() const {
    throw std::runtime_error("windows socket not implemented");
}


int CSocket::getID() const {
    throw std::runtime_error("windows socket not implemented");
}


size_t CSocket::write(Buffer::TSharedPtr const& buffer) {
    throw std::runtime_error("windows socket not implemented");
}


Buffer::TSharedPtr CSocket::read(size_t const &size) {
    throw std::runtime_error("windows socket not implemented");
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM
