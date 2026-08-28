#ifndef HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75
#define HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_acceptor.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortAcceptor CStreamPortAcceptor;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75
