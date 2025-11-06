#include <string>

#include "convert.h"


#ifdef MACOS_PLATFORM


using std::string;


namespace iridium::convertion::implementation {


string TConvert<string, long>::convert(long const &value) {
//    return TConvert<string, int64_t>::convert(static_cast<int64_t>(value)); // ----->
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<int64_t>(value), buffer, 10);
    return string(buffer);
}


string TConvert<string, unsigned long>::convert(unsigned long const &value) {
//    return TConvert<string, uint64_t>::convert(static_cast<uint64_t>(value)); // ----->
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<uint64_t>(value), buffer, 10);
    return string(buffer);
}


} // iridium::convertion::implementation


#endif // PLATFORM_MACOS
