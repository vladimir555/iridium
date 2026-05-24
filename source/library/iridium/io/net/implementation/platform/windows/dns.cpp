#include "dns.h"


#ifdef WINDOWS_PLATFORM


#include "wsa.h"


using std::string;


namespace iridium::io::net::implementation::platform {


URI::TIPv4 getIPv4ByHost(string const &host) {
    return WSA::instance().getIPv4ByName(host); // ----->
}


} // iridium::io::net::implementation::platform


#endif // WINDOWS_PLATFORM
