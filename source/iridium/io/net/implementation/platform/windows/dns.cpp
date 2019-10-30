/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "dns.h"


#ifdef WINDOWS_PLATFORM


#include "wsa.h"


using std::string;


namespace iridium {
namespace net {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(string const &host) {
    return WSA::instance().getIPv4ByName(host); // ----->
}


} // platform
} // implementation
} // net
} // iridium


#endif // WINDOWS_PLATFORM
