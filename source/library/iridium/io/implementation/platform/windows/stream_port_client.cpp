// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "stream_port_client.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port_acceptor.h"
#include "stream_port_peer.h"
#include "iridium/assert.h"
#include <stdexcept>


using iridium::assertExists;
using std::string;


namespace iridium::io::implementation::platform {


CStreamPortClient::CStreamPortClient(URI const &uri)
:
    CStreamPort(uri)
{
    throw std::runtime_error("not implemented");
}

void CStreamPortClient::initialize() {
    throw std::runtime_error("not implemented");
}


void CStreamPortClient::finalize() {
    throw std::runtime_error("not implemented");
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
