#ifndef HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
#define HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


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


#endif // HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
