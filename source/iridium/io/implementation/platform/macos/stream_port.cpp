#include "stream_port.h"


#ifdef MACOS_PLATFORM


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


CStreamPortSSL::CStreamPortSSL(URL const &url)
:
    platform::unix_::CStreamPort(url)
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
