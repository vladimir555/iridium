#include "listener.h"


#ifdef LINUX_PLATFORM


namespace utility {
namespace io {
namespace implementation {
namespace platform {


CListener::CListener() {
}


void CListener::initialize() {
}


void CListener::finalize() {
}


void CListener::add(IStream::TSharedPtr const &stream) {
}


void CListener::del(IStream::TSharedPtr const &stream) {
}


CListener::TEvents CListener::wait() {
    return {};
}


} // platform
} // implementation
} // io
} // utility


#endif // LINUX_PLATFORM
