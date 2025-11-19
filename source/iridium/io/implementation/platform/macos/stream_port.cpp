#include "stream_port.h"


#ifdef MACOS_PLATFORM


namespace iridium::io::implementation::platform {


CStreamPortSSL::CStreamPortSSL(URI const &uri)
:
    platform::unix_::CStreamPort(uri)
{}


void CStreamPortSSL::initialize() {

}


void CStreamPortSSL::finalize() {
}


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM
