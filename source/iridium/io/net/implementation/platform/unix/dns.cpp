#include "dns.h"


#ifdef UNIX_PLATFORM


#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "iridium/assert.h"
#include "iridium/io/uri.h"


using std::string;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


URI::TIPv4 getIPv4ByHost(string const &host) {
    auto host_entry = reinterpret_cast<in_addr const * const>(assertExists(
        gethostbyname(host.c_str()),
        "get ip by host name '" + host + "' error"));
    
    uint64_t ipv4_value = host_entry->s_addr;
//        reinterpret_cast<in_addr const * const>((void *)host_entry->h_addr_list[0])->s_addr;

    URI::TIPv4 ipv4;

    ipv4[0]     = ipv4_value &  0xFF;
    ipv4_value  = ipv4_value >> 8;
    ipv4[1]     = ipv4_value &  0xFF;
    ipv4_value  = ipv4_value >> 8;
    ipv4[2]     = ipv4_value &  0xFF;
    ipv4_value  = ipv4_value >> 8;
    ipv4[3]     = ipv4_value &  0xFF;

    return ipv4; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
