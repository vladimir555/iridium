#include "convert.h"


#ifdef LINUX_PLATFORM


namespace iridium::convertion::implementation {


std::string TConvert<std::string, long long>::convert(long long const &value) {
    return std::to_string(value);
}


} // iridium::convertion::implementation


#endif // LINUX_PLATFORM
