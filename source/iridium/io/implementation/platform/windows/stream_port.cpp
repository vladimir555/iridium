#include "stream_port.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


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


} // platform
} // implementation
} // io
} // iridium


#endif  // WINDOWS_PLATFORM
