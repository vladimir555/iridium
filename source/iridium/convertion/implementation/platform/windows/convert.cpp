/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <string>

#include "iridium/convertion/convert.h"

#include "convert.h"


#ifdef WINDOWS_PLATFORM


#include <stdlib.h>
#include <ctime>
#include <stdio.h>


using std::string;
using iridium::convertion::convert;


namespace iridium {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base) {
    ::_itoa_s(value, buffer, 64, base);
}


void itoa(uint32_t const &value, char *buffer, int const &base) {
    ::_itoa_s(value, buffer, 64, base);
}


void itoa(int64_t const &value, char *buffer, int const &base) {
    ::_i64toa_s(value, buffer, 64, base);
}


void itoa(uint64_t const &value, char *buffer, int const &base) {
    ::_ui64toa_s(value, buffer, 64, base);
}


::tm *gmtime(time_t const * const t) {
    struct tm *tm_ = new tm();
    ::gmtime_s(tm_, t);
    return tm_;
}


void gmtime_r(time_t* t, struct tm* tm_) {
    ::gmtime_s(tm_, t); // ----->
}


#ifdef PLATFORM_CPU32
__time32_t mkgmtime(struct tm * const tm_) {
    return ::_mkgmtime32(tm_); // ----->
}
#endif


#ifdef PLATFORM_CPU64
__time64_t mkgmtime(struct tm * const tm_) {
    return ::_mkgmtime64(tm_); // ----->
}
#endif


} // platform
} // implementation
} // convertuin
} // iridium


template<>
std::string iridium::convertion::implementation::convert(unsigned long const &value) {
    return convert<string>(static_cast<uint64_t>(value)); // ----->
}


#endif // WINDOWS_PLATFORM
