// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
#define HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPort CStreamPort;


} // iridium::io::implementation::platform


#endif // LINUX_PLATFORM


#endif // HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
