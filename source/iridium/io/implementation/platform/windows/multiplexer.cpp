#include "multiplexer.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


void CMultiplexer::initialize() {
    throw std::runtime_error("multiplexer is not implemented");
}


void CMultiplexer::finalize() {
    throw std::runtime_error("multiplexer is not implemented");
}


std::list<IEvent::TSharedPtr> CMultiplexer::waitEvents() {
    throw std::runtime_error("multiplexer is not implemented");
}

// todo: subscribe for IStreamReader and IStreamWriter
void CMultiplexer::subscribe(IStream::TConstSharedPtr const &stream) {
    throw std::runtime_error("multiplexer is not implemented");
}

// finalize and unsubscribe stream
void CMultiplexer::unsubscribe(IStream::TConstSharedPtr const &stream) {
    throw std::runtime_error("multiplexer is not implemented");
}


} // platform
} // implementation
} // io
} // iridium


#endif // WINDOWS_PLATFORM
