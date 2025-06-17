// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_1E0C9D96_3826_42B7_848B_C7D91E6E2B32
#define HEADER_SHARED_MEMORY_STREAM_PORT_1E0C9D96_3826_42B7_848B_C7D91E6E2B32


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


#include "../unix/shared_memory_stream_port.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CIPCStream CIPCStream;


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_1E0C9D96_3826_42B7_848B_C7D91E6E2B32
