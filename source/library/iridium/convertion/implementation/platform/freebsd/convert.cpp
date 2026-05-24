#include <string>

#include "convert.h"


#ifdef FREEBSD_PLATFORM


using std::string;


namespace iridium::convertion::implementation {


string TConvert<string, long long>::convert(long long const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<int64_t>(value), buffer, 10);
    return string(buffer);
}


} // iridium::convertion::implementation


#endif // FREEBSD_PLATFORM
