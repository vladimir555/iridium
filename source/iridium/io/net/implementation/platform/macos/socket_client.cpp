#include "socket_client.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketClient::CSocketClient(URL const &url)
:
    unix_::CSocketBase(url)
{}


void CSocketClient::initialize() {}


void CSocketClient::finalize() {}


URL CSocketClient::getURL() const {
    return *m_url;
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
