#include "dns.h"


#ifdef WINDOWS_PLATFORM


#include "wsa.h"


using std::string;


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(string const &host) {
    return WSA::instance().getIPv4ByName(host); // ----->
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // WINDOWS_PLATFORM
