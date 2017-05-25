#ifndef HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
#define HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0


#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#include <stdint.h>
#include <time.h>


namespace utility {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base);


void itoa(uint32_t const &value, char *buffer, int const &base);


void itoa(int64_t const &value, char *buffer, int const &base);


void itoa(uint64_t const &value, char *buffer, int const &base);


::tm *gmtime(time_t const * const t);


#ifdef CPU32
__time32_t mkgmtime(struct tm * const tm_);
#endif


#ifdef CPU64
__time64_t mkgmtime(struct tm * const tm_);
#endif


template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf_s(args ...); // ----->
}


__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, sprintf, vsprintf_s, _Post_z_ char, _Dest, _In_z_ _Printf_format_string_ const char *, _Format);


} // platform
} // implementation
} // convertuin 
} // utility


#endif // WINDOWS_PLATFORM


#endif // HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
