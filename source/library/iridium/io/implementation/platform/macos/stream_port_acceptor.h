#ifndef HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596
#define HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/stream_port_acceptor.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortAcceptor CStreamPortAcceptor;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596
