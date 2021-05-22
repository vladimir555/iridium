#include "socket_client.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocketClient::CSocketClient(URL const& url, IListener::TSharedPtr const& listener) {
    throw std::runtime_error("not implemented");
}

void CSocketClient::initialize() {
}


void CSocketClient::finalize() {
}


URL CSocketClient::getURL() const {
	return URL("");
}


int CSocketClient::getID() const {
	return {};
}


Buffer::TSharedPtr CSocketClient::read(size_t const& size) {
	return {};
}


size_t CSocketClient::write(Buffer::TSharedPtr const& buffer) {
	return {};
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM
