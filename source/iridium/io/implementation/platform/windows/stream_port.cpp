#include "stream_port.h"


#ifdef WINDOWS_PLATFORM


namespace iridium::io::implementation::platform {


CStreamPort::CStreamPort(URI const &uri) {}


void CStreamPort::initialize() {
    throw std::runtime_error("stream port is not implemented");
}


void CStreamPort::finalize() {
    throw std::runtime_error("stream port is not implemented");
}


std::list<uintptr_t> CStreamPort::getHandles() const {
    throw std::runtime_error("stream port is not implemented");
}


Buffer::TSharedPtr CStreamPort::read(size_t const &size) {
    throw std::runtime_error("stream port is not implemented");
}


size_t CStreamPort::write(Buffer::TSharedPtr const &buffer) {
    throw std::runtime_error("stream port is not implemented");
}


} // iridium::io::implementation::platform


#endif  // WINDOWS_PLATFORM
