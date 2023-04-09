#ifndef HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
#define HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


#include "../unix/process.h"


namespace iridium {
namespace system {
namespace implementation {
namespace platform {


typedef platform::unix_::CProcessStream CProcessStream;


} // platform
} // implementation
} // system
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
