#include "socket_client.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketClient::CSocketClient(URI const &uri)
:
    unix_::CSocketBase(uri)
{}


void CSocketClient::initialize() {}


void CSocketClient::finalize() {}


URI::TSharedPtr CSocketClient::getURI() const {
    return m_uri;
}


int CSocketClient::getID() const {
    return 0;
}


Buffer::TSharedPtr CSocketClient::read(size_t const &size) {
    return {};
}


size_t CSocketClient::write(Buffer::TSharedPtr const &buffer) {
    return 0;
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM
