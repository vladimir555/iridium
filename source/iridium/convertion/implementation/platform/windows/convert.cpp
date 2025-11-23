// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements Windows-specific type conversion specializations.
/// \~russian @file
/// @brief Реализует платформо-специфичные специализации преобразования типов для Windows.

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

/// \~english @brief Converts a 32-bit signed integer to a string.
/// \~russian @brief Преобразует 32-битное знаковое целое число в строку.
void itoa(int32_t const &value, char *buffer, int const &base) {
    ::_itoa_s(value, buffer, 64, base);
}

/// \~english @brief Converts a 32-bit unsigned integer to a string.
/// \~russian @brief Преобразует 32-битное беззнаковое целое число в строку.
void itoa(uint32_t const &value, char *buffer, int const &base) {
    ::_itoa_s(value, buffer, 64, base);
}

/// \~english @brief Converts a 64-bit signed integer to a string.
/// \~russian @brief Преобразует 64-битное знаковое целое число в строку.
void itoa(int64_t const &value, char *buffer, int const &base) {
    ::_i64toa_s(value, buffer, 64, base);
}

/// \~english @brief Converts a 64-bit unsigned integer to a string.
/// \~russian @brief Преобразует 64-битное беззнаковое целое число в строку.
void itoa(uint64_t const &value, char *buffer, int const &base) {
    ::_ui64toa_s(value, buffer, 64, base);
}

/// \~english @brief A wrapper for the `gmtime_s` function.
/// \~russian @brief Обертка для функции `gmtime_s`.
::tm *gmtime(time_t const * const t) {
    struct tm *tm_ = new tm();
    ::gmtime_s(tm_, t);
    return tm_;
}

/// \~english @brief A wrapper for the `gmtime_s` function.
/// \~russian @brief Обертка для функции `gmtime_s`.
void gmtime_r(time_t* t, struct tm* tm_) {
    ::gmtime_s(tm_, t); // ----->
}

/// \~english @brief A wrapper for the `localtime_s` function.
/// \~russian @brief Обертка для функции `localtime_s`.
void localtime_r(time_t *t, struct tm *tm_) {
    ::localtime_s(tm_, t); // ----->
}

#ifdef PLATFORM_CPU32
/// \~english @brief A wrapper for the `_mkgmtime32` function.
/// \~russian @brief Обертка для функции `_mkgmtime32`.
__time32_t mkgmtime(struct tm * const tm_) {
    return ::_mkgmtime32(tm_); // ----->
}
#endif

#ifdef PLATFORM_CPU64
/// \~english @brief A wrapper for the `_mkgmtime64` function.
/// \~russian @brief Обертка для функции `_mkgmtime64`.
__time64_t mkgmtime(struct tm * const tm_) {
    return ::_mkgmtime64(tm_); // ----->
}
#endif

} // iridium::convertion::implementation::platform

#endif // WINDOWS_PLATFORM
