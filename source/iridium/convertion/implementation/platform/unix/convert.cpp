// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements Unix-specific integer to string conversion functions.
/// \~russian @file
/// @brief Реализует платформо-специфичные функции преобразования целых чисел в строку для Unix.

#include "convert.h"

#ifdef UNIX_PLATFORM

#include <algorithm>
#include <string>

using std::string;

namespace {

/// \~english @brief Converts an unsigned integer to a string.
/// \~russian @brief Преобразует беззнаковое целое число в строку.
template<typename TUInt>
void itoaUInt(TUInt const &value_, char *buffer, uint8_t const &base) {
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

/// \~english @brief Converts a signed integer to a string.
/// \~russian @brief Преобразует знаковое целое число в строку.
template<typename TInt>
void itoaInt(TInt const &value_, char *buffer, uint8_t const &base) {
    TInt value = value_;

    if (value_ < 0) {
       *buffer = '-';
        buffer++;
        value = -value;
    }

    itoaUInt(value, buffer, base);
}

} // unnamed

namespace iridium::convertion::implementation::platform {

/// \~english @brief Converts a 32-bit signed integer to a string.
/// \~russian @brief Преобразует 32-битное знаковое целое число в строку.
void itoa(int32_t const &value, char *buffer, uint8_t const &base) {
    itoaInt(value, buffer, base);
}

/// \~english @brief Converts a 64-bit signed integer to a string.
/// \~russian @brief Преобразует 64-битное знаковое целое число в строку.
void itoa(int64_t const &value, char *buffer, uint8_t const &base) {
    itoaInt(value, buffer, base);
}

/// \~english @brief Converts a 32-bit unsigned integer to a string.
/// \~russian @brief Преобразует 32-битное беззнаковое целое число в строку.
void itoa(uint32_t const &value, char *buffer, uint8_t const &base) {
    itoaUInt(value, buffer, base);
}

/// \~english @brief Converts a 64-bit unsigned integer to a string.
/// \~russian @brief Преобразует 64-битное беззнаковое целое число в строку.
void itoa(uint64_t const &value, char *buffer, uint8_t const &base) {
    itoaUInt(value, buffer, base);
}

} // iridium::convertion::implementation::platform

#endif // UNIX_PLATFORM
