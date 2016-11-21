#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctime>


//#include <utility/macros/disable_warnings.h>


namespace utility {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base) {
    _itoa(value, buffer, base);
}


void itoa(int64_t const &value, char *buffer, int const &base) {
    _i64toa(value, buffer, base);
}


auto timezone = ::_timezone;
auto sscanf   = ::sscanf;
auto gmtime   = ::gmtime;
auto sprintf  = ::sprintf;
auto mkgmtime = ::mkgmtime; //todo: 32 64 func


} // platform
} // implementation
} // convertion
} // utility


//#include <utility/macros/enable_warnings.h>
