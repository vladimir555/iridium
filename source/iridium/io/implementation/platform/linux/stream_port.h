#ifndef HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
#define HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix::CStreamPort CStreamPort;


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
