#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF
#define HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_client.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortClient CStreamPortClient;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF
