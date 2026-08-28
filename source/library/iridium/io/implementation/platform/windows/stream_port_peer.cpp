// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

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
