#ifndef HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580
#define HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#include "utility/networking/types.h"


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(std::string const &host);


} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#endif // WINDOWS_PLATFORM
