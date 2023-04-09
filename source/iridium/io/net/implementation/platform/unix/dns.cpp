#include "dns.h"


#ifdef UNIX_PLATFORM


#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "iridium/assert.h"


using std::string;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


TIPv4 getIPv4ByHost(string const &host) {
    auto host_entry = assertExists(
        gethostbyname(host.c_str()),
        "get ip by host name '" + host + "' error");
    
    uint64_t ipv4_value =
        reinterpret_cast<in_addr const * const>(host_entry->h_addr_list[0])->s_addr;

    TIPv4 ipv4;

    ipv4.push_back(ipv4_value & 0xFF);
    ipv4_value = ipv4_value >> 8;
    ipv4.push_back(ipv4_value & 0xFF);
    ipv4_value = ipv4_value >> 8;
    ipv4.push_back(ipv4_value & 0xFF);
    ipv4_value = ipv4_value >> 8;
    ipv4.push_back(ipv4_value & 0xFF);

    return ipv4; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
