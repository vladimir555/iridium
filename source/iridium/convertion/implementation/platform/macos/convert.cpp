#include <string>

#include "convert.h"


#ifdef MACOS_PLATFORM


using std::string;


namespace iridium::convertion::implementation {


string TConvert<string, long>::convert(long const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<int64_t>(value), buffer, base);
    return string(buffer);
}


string TConvert<string, unsigned long>::convert(unsigned long const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<uint64_t>(value), buffer, base);
    return string(buffer);
}


long TConvert<long, string>::convert(string const &value, uint8_t const &base) {
    char *end = nullptr;
    long result = strtol(value.c_str(), &end, base);
    return result;
}


unsigned long TConvert<unsigned long, string>::convert(string const &value, uint8_t const &base) {
    char *end = nullptr;
    long result = strtoul(value.c_str(), &end, base);
    return result;
}


} // iridium::convertion::implementation


#endif // MACOS_PLATFORM
