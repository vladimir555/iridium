#ifndef HEADER_STREAM_PORT_111226A9_81AA_467F_86C2_012D8B6D30B6
#define HEADER_STREAM_PORT_111226A9_81AA_467F_86C2_012D8B6D30B6


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


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


#endif // FREEBSD_PLATFORM


#endif // HEADER_STREAM_PORT_111226A9_81AA_467F_86C2_012D8B6D30B6
