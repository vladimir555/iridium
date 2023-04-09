// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586
#define HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


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


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586
