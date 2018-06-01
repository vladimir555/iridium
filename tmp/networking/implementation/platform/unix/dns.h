#ifndef HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33
#define HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#include "utility/platform.h"


#ifdef UNIX_PLATFORM


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


#endif // HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#endif // UNIX_PLATFORM
