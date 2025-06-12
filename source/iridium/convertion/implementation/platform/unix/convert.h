// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#include "iridium/platform.h"


/// \~english @brief This entire header provides Unix-platform specific helper functions for type conversions.
///     The declarations within are only compiled if `UNIX_PLATFORM` is defined.
/// \~russian @brief Весь этот заголовочный файл предоставляет специфичные для Unix-платформы вспомогательные функции для преобразования типов.
///     Объявления внутри компилируются только если определен макрос `UNIX_PLATFORM`.
#ifdef UNIX_PLATFORM


/// \~english @brief Header guard for Unix-specific conversion helper functions.
/// \~russian @brief Защита заголовка для специфичных для Unix вспомогательных функций преобразования.
#ifndef HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411
#define HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411


#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>


namespace iridium {
namespace convertion {
namespace implementation {
/// \~english @brief Namespace for platform-specific implementation details, here for Unix-like systems.
/// \~russian @brief Пространство имен для платформо-специфичных деталей реализации, здесь для Unix-подобных систем.
namespace platform {


/// \~english @brief Converts a 32-bit signed integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. The buffer must be large enough.
/// \~russian @brief Преобразует 32-битное знаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Буфер должен быть достаточно большим.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка.
/// \~english @param base Numerical base used to represent the value as a string (e.g., 10 for decimal, 16 for hexadecimal).
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки (например, 10 для десятичного, 16 для шестнадцатеричного).
void itoa(int32_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 32-bit unsigned integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. The buffer must be large enough.
/// \~russian @brief Преобразует 32-битное беззнаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Буфер должен быть достаточно большим.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка.
/// \~english @param base Numerical base used to represent the value as a string (e.g., 10 for decimal, 16 for hexadecimal).
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки (например, 10 для десятичного, 16 для шестнадцатеричного).
void itoa(uint32_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 64-bit signed integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. The buffer must be large enough.
/// \~russian @brief Преобразует 64-битное знаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Буфер должен быть достаточно большим.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка.
/// \~english @param base Numerical base used to represent the value as a string (e.g., 10 for decimal, 16 for hexadecimal).
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки (например, 10 для десятичного, 16 для шестнадцатеричного).
void itoa(int64_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 64-bit unsigned integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. The buffer must be large enough.
/// \~russian @brief Преобразует 64-битное беззнаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Буфер должен быть достаточно большим.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка.
/// \~english @param base Numerical base used to represent the value as a string (e.g., 10 for decimal, 16 for hexadecimal).
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки (например, 10 для десятичного, 16 для шестнадцатеричного).
void itoa(uint64_t const &value, char *buffer, int const &base);

/// \~english @brief Template wrapper for the standard C library function `sscanf`.
///     Provides a type-safe way to call `sscanf` with variadic arguments.
/// \~russian @brief Шаблонная обертка для стандартной функции библиотеки C `sscanf`.
///     Предоставляет типобезопасный способ вызова `sscanf` с переменным числом аргументов.
/// \~english @tparam TArgs Argument types for `sscanf`.
/// \~russian @tparam TArgs Типы аргументов для `sscanf`.
/// \~english @param args Arguments to be passed to `::sscanf`.
/// \~russian @param args Аргументы для передачи в `::sscanf`.
/// \~english @return The value returned by `::sscanf`.
/// \~russian @return Значение, возвращаемое `::sscanf`.
template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf(args ...);
}

/// \~english @brief Template wrapper for the standard C library function `gmtime`.
///     Provides a type-safe way to call `gmtime` with its argument.
/// \~russian @brief Шаблонная обертка для стандартной функции библиотеки C `gmtime`.
///     Предоставляет типобезопасный способ вызова `gmtime` с ее аргументом.
/// \~english @tparam TArgs Argument types for `gmtime` (expected to be `const time_t*`).
/// \~russian @tparam TArgs Типы аргументов для `gmtime` (ожидается `const time_t*`).
/// \~english @param args Arguments to be passed to `::gmtime`.
/// \~russian @param args Аргументы для передачи в `::gmtime`.
/// \~english @return The value returned by `::gmtime` (a pointer to a statically allocated `struct tm`).
/// \~russian @return Значение, возвращаемое `::gmtime` (указатель на статически размещенную `struct tm`).
template<typename ... TArgs>
struct tm *gmtime(TArgs ... args) {
    return ::gmtime(args ...);
}

/// \~english @brief Template wrapper for the POSIX function `gmtime_r`.
///     Provides a type-safe way to call `gmtime_r` with its arguments. `gmtime_r` is a thread-safe version of `gmtime`.
/// \~russian @brief Шаблонная обертка для функции POSIX `gmtime_r`.
///     Предоставляет типобезопасный способ вызова `gmtime_r` с ее аргументами. `gmtime_r` является потокобезопасной версией `gmtime`.
/// \~english @tparam TArgs Argument types for `gmtime_r` (expected to be `const time_t*` and `struct tm*`).
/// \~russian @tparam TArgs Типы аргументов для `gmtime_r` (ожидаются `const time_t*` и `struct tm*`).
/// \~english @param args Arguments to be passed to `::gmtime_r`.
/// \~russian @param args Аргументы для передачи в `::gmtime_r`.
template<typename ... TArgs>
void gmtime_r(TArgs ... args) {
    ::gmtime_r(args ...);
}

/// \~english @brief Template wrapper for the POSIX function `localtime_r`.
///     Provides a type-safe way to call `localtime_r` with its arguments. `localtime_r` is a thread-safe version of `localtime`.
/// \~russian @brief Шаблонная обертка для функции POSIX `localtime_r`.
///     Предоставляет типобезопасный способ вызова `localtime_r` с ее аргументами. `localtime_r` является потокобезопасной версией `localtime`.
/// \~english @tparam TArgs Argument types for `localtime_r` (expected to be `const time_t*` and `struct tm*`).
/// \~russian @tparam TArgs Типы аргументов для `localtime_r` (ожидаются `const time_t*` и `struct tm*`).
/// \~english @param args Arguments to be passed to `::localtime_r`.
/// \~russian @param args Аргументы для передачи в `::localtime_r`.
template<typename ... TArgs>
void localtime_r(TArgs ... args) {
    ::localtime_r(args ...);
}

/// \~english @brief Template wrapper for the standard C library function `snprintf`.
///     Provides a type-safe way to call `snprintf` with variadic arguments.
/// \~russian @brief Шаблонная обертка для стандартной функции библиотеки C `snprintf`.
///     Предоставляет типобезопасный способ вызова `snprintf` с переменным числом аргументов.
/// \~english @tparam TArgs Argument types for `snprintf`.
/// \~russian @tparam TArgs Типы аргументов для `snprintf`.
/// \~english @param args Arguments to be passed to `::snprintf`.
/// \~russian @param args Аргументы для передачи в `::snprintf`.
/// \~english @return The value returned by `::snprintf` (number of characters that would have been written).
/// \~russian @return Значение, возвращаемое `::snprintf` (количество символов, которое было бы записано).
template<typename ... TArgs>
int snprintf(TArgs ... args) {
    return ::snprintf(args ...);
}

/// \~english @brief Template wrapper for the `timegm` function (a common GNU/BSD extension).
///     Converts a `struct tm` (representing UTC) into a `time_t`.
/// \~russian @brief Шаблонная обертка для функции `timegm` (распространенное расширение GNU/BSD).
///     Преобразует `struct tm` (представляющую UTC) в `time_t`.
/// \~english @tparam TArgs Argument types for `timegm` (expected to be `struct tm*`).
/// \~russian @tparam TArgs Типы аргументов для `timegm` (ожидается `struct tm*`).
/// \~english @param args Arguments to be passed to `::timegm`.
/// \~russian @param args Аргументы для передачи в `::timegm`.
/// \~english @return The `time_t` value corresponding to the input `struct tm`.
/// \~russian @return Значение `time_t`, соответствующее входной структуре `struct tm`.
template<typename ... TArgs>
time_t mkgmtime(TArgs ... args) {
    return ::timegm(args ...); // Note: timegm is not standard C or POSIX, but a common extension.
}


} // platform
} // implementation
} // convertion
} // iridium


#endif // HEADER_CONVERT_0B3F9A65_12EC_4018_A94C_D2DE013D2411


#endif // UNIX_PLATFORM
