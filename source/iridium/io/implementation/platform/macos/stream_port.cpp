#include "stream_port.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


CStreamPortSSL::CStreamPortSSL(URI const &uri)
:
    platform::unix_::CStreamPort(uri)
{}


void CStreamPortSSL::initialize() {
   
}


void CStreamPortSSL::finalize() {
}


} // platform
} // implementation
} // io
} // iridium


#endif // MACOS_PLATFORM
