#include "utility/platform.h"



#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)


#ifndef HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411
#define HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411


#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>


namespace utility {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base);


void itoa(uint32_t const &value, char *buffer, int const &base);


void itoa(int64_t const &value, char *buffer, int const &base);


void itoa(uint64_t const &value, char *buffer, int const &base);


template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf(args ...); // ----->
}


//auto gmtime      = ::gmtime;
//auto sprintf     = ::sprintf;
//auto mkgmtime    = ::timegm;


template<typename ... TArgs>
struct tm *gmtime(TArgs ... args) {
    return ::gmtime(args ...); // ----->
}


template<typename ... TArgs>
int sprintf(TArgs ... args) {
    return ::sprintf(args ...); // ----->
}


template<typename ... TArgs>
time_t mkgmtime(TArgs ... args) {
    return ::timegm(args ...); // ----->
}


} // platform
} // implementation
} // convertuin 
} // utility


#endif // HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411


#endif // LINUX_PLATFORM FREEBSD_PLATFORM MACOS_PLATFORM
