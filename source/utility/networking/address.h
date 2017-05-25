#ifndef HEADER_ADDRESS_3F6003E0_9A9B_4B6E_B9D0_6928A962DC4B
#define HEADER_ADDRESS_3F6003E0_9A9B_4B6E_B9D0_6928A962DC4B


#include <string>


namespace utility {
namespace networking {


///
struct TAddressIPv4 {
    ///
    int ip[4];
    ///
    int port;
    ///
    TAddressIPv4(std::string const &address);
};


} // networking
} // utility


#endif // HEADER_ADDRESS_3F6003E0_9A9B_4B6E_B9D0_6928A962DC4B
