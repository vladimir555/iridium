#include "dns.h"


#ifdef WINDOWS_PLATFORM


#include "wsa.h"


using std::string;


namespace utility {
namespace net {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(string const &host) {
    return WSA::instance().getIPv4ByName(host); // ----->
}


} // platform
} // implementation
} // net
} // utility


#else
void dummy() {}
#endif // WINDOWS_PLATFORM
