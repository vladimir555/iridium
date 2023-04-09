#ifndef HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
#define HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


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


#endif // MACOS_PLATFORM


#endif // HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
