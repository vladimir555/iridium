#include "utility/platform.h"



#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)


#include "convert.h"

#include <algorithm>


namespace {


template<typename TUInt>
void itoaUInt(TUInt const &value_, char *buffer, int const &base) {
    TUInt value = value_;

    char* buffer_begin = buffer;
    do {
        char ch = value % base;
        if (ch > 9)
            ch = 'A' - 10 + ch;
        else
            ch = '0' + ch;

       *buffer = ch;
        buffer++;
        value /= base;
    } while (value);
   *buffer = 0;
    std::reverse(buffer_begin, buffer);
}


template<typename TInt>
void itoaInt(TInt const &value_, char *buffer, int const &base) {
    TInt value = value_;

    if (value_ < 0) {
       *buffer = '-';
        buffer++;
        value = -value;
    }

    itoaUInt(value, buffer, base);
}


} // unnamed


namespace utility {
namespace convertion {
namespace implementation {
namespace platform {


void itoa(int32_t const &value_, char *buffer, int const &base) {
    // code copies for fix clang bug
    uint32_t value = value_;

    if (value_ < 0) {
       *buffer = '-';
        buffer++;
        value = -value_;
    }

    char* buffer_begin = buffer;
    do {
        char ch = value % base;
        if (ch > 9)
            ch = 'A' - 10 + ch;
        else
            ch = '0' + ch;

       *buffer = ch;
        buffer++;
        value /= base;
    } while (value);
   *buffer = 0;
    std::reverse(buffer_begin, buffer);
}


void itoa(int64_t const &value_, char *buffer, int const &base) {
    // code copies for fix clang bug
    uint64_t value = value_;

    if (value_ < 0) {
       *buffer = '-';
        buffer++;
        value = -value_;
    }

    char* buffer_begin = buffer;
    do {
        char ch = value % base;
        if (ch > 9)
            ch = 'A' - 10 + ch;
        else
            ch = '0' + ch;

       *buffer = ch;
        buffer++;
        value /= base;
    } while (value);
   *buffer = 0;
    std::reverse(buffer_begin, buffer);
}


void itoa(uint32_t const &value, char *buffer, int const &base) {
    itoaUInt(value, buffer, base);
}


void itoa(uint64_t const &value, char *buffer, int const &base) {
    itoaUInt(value, buffer, base);
}


} // platform
} // implementation
} // convertuin
} // utility


#endif // LINUX_PLATFORM FREEBSD_PLATFORM MACOS_PLATFORM
