// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
#define HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include <stdint.h>
#include <time.h>


namespace iridium {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base);


void itoa(uint32_t const &value, char *buffer, int const &base);


void itoa(int64_t const &value, char *buffer, int const &base);


void itoa(uint64_t const &value, char *buffer, int const &base);


::tm *gmtime(time_t const * const t);


void gmtime_r(time_t* t, struct tm* tm_);


void localtime_r(time_t *t, struct tm *tm_);


#ifdef PLATFORM_CPU32
__time32_t mkgmtime(struct tm * const tm_);
#endif


#ifdef PLATFORM_CPU64
__time64_t mkgmtime(struct tm * const tm_);
#endif


template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf_s(args ...); // ----->
}


template<typename ... TArgs>
int snprintf(TArgs ... args) {
    return ::snprintf(args ...); // ----->
}


__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, sprintf, vsprintf_s, _Post_z_ char, _Dest, _In_z_ _Printf_format_string_ const char *, _Format);


} // platform
} // implementation
} // convertuin 
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
