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
