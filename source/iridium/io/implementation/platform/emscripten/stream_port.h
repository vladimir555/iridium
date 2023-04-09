// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF
#define HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF


#include "iridium/platform.h"


#ifdef EMSCRIPTEN_PLATFORM


#include "../unix/stream_port.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CStreamPort CStreamPort;


} // platform
} // implementation
} // io
} // iridium


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF
