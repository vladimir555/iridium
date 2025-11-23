// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411
#define HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411


/// \~english @file
/// @brief Declares Unix-specific conversion functions.
/// \~russian @file
/// @brief Объявляет платформо-специфичные функции преобразования для Unix.


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/convertion/implementation/common.h"

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>


namespace iridium::convertion::implementation::platform {


/// \~english @brief Converts a 32-bit signed integer to a string.
/// \~russian @brief Преобразует 32-битное знаковое целое число в строку.
void itoa(int32_t const &value, char *buffer, uint8_t const &base);


/// \~english @brief Converts a 32-bit unsigned integer to a string.
/// \~russian @brief Преобразует 32-битное беззнаковое целое число в строку.
void itoa(uint32_t const &value, char *buffer, uint8_t const &base);


/// \~english @brief Converts a 64-bit signed integer to a string.
/// \~russian @brief Преобразует 64-битное знаковое целое число в строку.
void itoa(int64_t const &value, char *buffer, uint8_t const &base);


/// \~english @brief Converts a 64-bit unsigned integer to a string.
/// \~russian @brief Преобразует 64-битное беззнаковое целое число в строку.
void itoa(uint64_t const &value, char *buffer, uint8_t const &base);


/// \~english @brief A wrapper for the `sscanf` function.
/// \~russian @brief Обертка для функции `sscanf`.
template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf(args ...); // ----->
}


/// \~english @brief A wrapper for the `gmtime` function.
/// \~russian @brief Обертка для функции `gmtime`.
template<typename ... TArgs>
struct tm *gmtime(TArgs ... args) {
    return ::gmtime(args ...); // ----->
}


/// \~english @brief A wrapper for the `gmtime_r` function.
/// \~russian @brief Обертка для функции `gmtime_r`.
template<typename ... TArgs>
void gmtime_r(TArgs ... args) {
    ::gmtime_r(args ...); // ----->
}


/// \~english @brief A wrapper for the `localtime_r` function.
/// \~russian @brief Обертка для функции `localtime_r`.
template<typename ... TArgs>
void localtime_r(TArgs ... args) {
    ::localtime_r(args ...); // ----->
}


/// \~english @brief A wrapper for the `snprintf` function.
/// \~russian @brief Обертка для функции `snprintf`.
template<typename ... TArgs>
int snprintf(TArgs ... args) {
    return ::snprintf(args ...); // ----->
}


/// \~english @brief A wrapper for the `timegm` function.
/// \~russian @brief Обертка для функции `timegm`.
template<typename ... TArgs>
time_t mkgmtime(TArgs ... args) {
    return ::timegm(args ...); // ----->
}


} // iridium::convert::implementation


#endif // UNIX_PLATFORM


#endif // HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411
