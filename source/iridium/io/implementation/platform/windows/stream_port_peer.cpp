#include "stream_port_peer.h"


#ifdef WINDOWS_PLATFORM


namespace iridium::io::implementation::platform {


CStreamPortPeer::CStreamPortPeer(URI const &uri)
:
    CStreamPort(uri)
{}


CStreamPortPeer::CStreamPortPeer(URI const &uri, int const &/*fd*/)
:
    CStreamPort(uri)
{
    throw std::runtime_error("not implemented");
}


void CStreamPortPeer::initialize() {
    throw std::runtime_error("not implemented");
}


void CStreamPortPeer::finalize() {
    throw std::runtime_error("not implemented");
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
