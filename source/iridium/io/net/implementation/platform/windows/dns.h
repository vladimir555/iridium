#ifndef HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580
#define HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/net/types.h"


namespace iridium {
namespace net {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(std::string const &host);


} // platform
} // implementation
} // net
} // iridium


#endif // HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#endif // WINDOWS_PLATFORM
