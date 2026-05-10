#include "convert.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/convertion/convert.h"

#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <string>


using std::string;
using iridium::convertion::convert;


namespace iridium::convertion::implementation::platform {


void itoa(int32_t const &value, char *buffer, int const &base) {
    if (base < 2 || base > 64)
        throw std::invalid_argument("convertion int32_t error: wrong base " + std::to_string(base));

    ::_itoa_s(value, buffer, 64, base);
}


void itoa(uint32_t const &value, char *buffer, int const &base) {
    if (base < 2 || base > 64)
        throw std::invalid_argument("convertion uint32_t error: wrong base " + std::to_string(base));

    ::_itoa_s(value, buffer, 64, base);
}


void itoa(int64_t const &value, char *buffer, int const &base) {
    if (base < 2 || base > 64)
        throw std::invalid_argument("convertion int64_t error: wrong base " + std::to_string(base));

    ::_i64toa_s(value, buffer, 64, base);
}


void itoa(uint64_t const &value, char *buffer, int const &base) {
    if (base < 2 || base > 64)
        throw std::invalid_argument("convertion uint64_t error: wrong base " + std::to_string(base));

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


void localtime_r(time_t *t, struct tm *tm_) {
    ::localtime_s(tm_, t); // ----->
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


} // iridium::convertion::implementation::platform


#endif // WINDOWS_PLATFORM
