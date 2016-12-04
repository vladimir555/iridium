#include <stdint.h>


namespace {


template<typename TInt>
void itoaInternal(TInt value, char *buffer, int const &base) {
    if (value < 0) {
       *buffer = '-';
        buffer++;
        value = -value;
    }
    do {
        char ch = value % base;
        if (ch > 9)
            ch = 'A' - 10 + ch;
        else
            ch = '0' + ch;
       *buffer = ch;
        value /= base;
        buffer++;
    } while (value);
   *buffer = 0;
}


} // unnamed


namespace utility {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value, char *buffer, int const &base) {
    itoaInternal(value, buffer, base);
}


void itoa(int64_t const &value, char *buffer, int const &base) {
    itoaInternal(value, buffer, base);
}


auto timezone = ::timezone;
auto sscanf   = ::sscanf;
auto gmtime   = ::gmtime;
auto sprintf  = ::sprintf;
auto mkgmtime = ::timegm;


} // platform
} // implementation
} // convertion
} // utility
