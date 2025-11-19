// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
#define HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include <stdint.h>
#include <time.h>


namespace iridium::convertion::implementation::platform {


/// \~english @brief Converts a 32-bit signed integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. Likely implemented using `_itoa_s` or similar on Windows.
/// \~russian @brief Преобразует 32-битное знаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Вероятно, реализовано с использованием `_itoa_s` или аналогичной функции в Windows.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored. Must be large enough.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка. Должен быть достаточно большим.
/// \~english @param base Numerical base used to represent the value as a string (e.g., 10 for decimal, 16 for hexadecimal).
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки (например, 10 для десятичного, 16 для шестнадцатеричного).
void itoa(int32_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 32-bit unsigned integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. Likely implemented using `_ultoa_s` or similar on Windows.
/// \~russian @brief Преобразует 32-битное беззнаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Вероятно, реализовано с использованием `_ultoa_s` или аналогичной функции в Windows.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored. Must be large enough.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка. Должен быть достаточно большим.
/// \~english @param base Numerical base used to represent the value as a string.
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки.
void itoa(uint32_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 64-bit signed integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. Likely implemented using `_i64toa_s` or similar on Windows.
/// \~russian @brief Преобразует 64-битное знаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Вероятно, реализовано с использованием `_i64toa_s` или аналогичной функции в Windows.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored. Must be large enough.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка. Должен быть достаточно большим.
/// \~english @param base Numerical base used to represent the value as a string.
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки.
void itoa(int64_t const &value, char *buffer, int const &base);

/// \~english @brief Converts a 64-bit unsigned integer to a null-terminated string using the specified base.
///     The resulting string is stored in the provided buffer. Likely implemented using `_ui64toa_s` or similar on Windows.
/// \~russian @brief Преобразует 64-битное беззнаковое целое число в строку с нулевым символом в конце, используя указанное основание.
///     Результирующая строка сохраняется в предоставленном буфере. Вероятно, реализовано с использованием `_ui64toa_s` или аналогичной функции в Windows.
/// \~english @param value The integer value to convert.
/// \~russian @param value Целочисленное значение для преобразования.
/// \~english @param buffer Pointer to a character array where the resulting string is stored. Must be large enough.
/// \~russian @param buffer Указатель на символьный массив, где сохраняется результирующая строка. Должен быть достаточно большим.
/// \~english @param base Numerical base used to represent the value as a string.
/// \~russian @param base Числовое основание, используемое для представления значения в виде строки.
void itoa(uint64_t const &value, char *buffer, int const &base);

/// \~english @brief Windows-specific wrapper or declaration for `gmtime`.
///     Converts a `time_t` value to a `struct tm` representing UTC.
///     Note: Standard `gmtime` is not thread-safe. Windows may provide `gmtime_s` which is safer.
/// \~russian @brief Специфичная для Windows обертка или объявление для `gmtime`.
///     Преобразует значение `time_t` в `struct tm`, представляющую UTC.
///     Примечание: Стандартная функция `gmtime` не является потокобезопасной. Windows может предоставлять `gmtime_s`, которая безопаснее.
/// \~english @param t Pointer to the `time_t` value to convert.
/// \~russian @param t Указатель на значение `time_t` для преобразования.
/// \~english @return Pointer to a statically allocated `struct tm`. This pointer may be invalidated by subsequent calls to `gmtime` or other time functions.
/// \~russian @return Указатель на статически размещенную `struct tm`. Этот указатель может стать недействительным при последующих вызовах `gmtime` или других функций времени.
::tm *gmtime(time_t const * const t);

/// \~english @brief Provides a `gmtime_r`-like (reentrant/thread-safe) interface for Windows.
///     Converts a `time_t` value to `struct tm` (UTC) and stores it in the user-provided `tm_` buffer.
///     Likely implemented using `gmtime_s` on Windows.
/// \~russian @brief Предоставляет `gmtime_r`-подобный (повторно входимый/потокобезопасный) интерфейс для Windows.
///     Преобразует значение `time_t` в `struct tm` (UTC) и сохраняет его в предоставленном пользователем буфере `tm_`.
///     Вероятно, реализовано с использованием `gmtime_s` в Windows.
/// \~english @param t Pointer to the `time_t` value to convert.
/// \~russian @param t Указатель на значение `time_t` для преобразования.
/// \~english @param tm_ Pointer to a `struct tm` object where the result will be stored.
/// \~russian @param tm_ Указатель на объект `struct tm`, где будет сохранен результат.
void gmtime_r(time_t* t, struct tm* tm_);

/// \~english @brief Provides a `localtime_r`-like (reentrant/thread-safe) interface for Windows.
///     Converts a `time_t` value to `struct tm` (local time) and stores it in the user-provided `tm_` buffer.
///     Likely implemented using `localtime_s` on Windows.
/// \~russian @brief Предоставляет `localtime_r`-подобный (повторно входимый/потокобезопасный) интерфейс для Windows.
///     Преобразует значение `time_t` в `struct tm` (локальное время) и сохраняет его в предоставленном пользователем буфере `tm_`.
///     Вероятно, реализовано с использованием `localtime_s` в Windows.
/// \~english @param t Pointer to the `time_t` value to convert.
/// \~russian @param t Указатель на значение `time_t` для преобразования.
/// \~english @param tm_ Pointer to a `struct tm` object where the result will be stored.
/// \~russian @param tm_ Указатель на объект `struct tm`, где будет сохранен результат.
void localtime_r(time_t *t, struct tm *tm_);

/// \~english @brief Converts a `struct tm` (representing UTC) into a 32-bit `time_t` value on 32-bit Windows platforms.
///     This is a Windows-specific equivalent to the non-standard `timegm` function.
/// \~russian @brief Преобразует `struct tm` (представляющую UTC) в 32-битное значение `time_t` на 32-битных платформах Windows.
///     Это специфичный для Windows эквивалент нестандартной функции `timegm`.
/// \~english @param tm_ Pointer to the `struct tm` object (in UTC) to convert.
/// \~russian @param tm_ Указатель на объект `struct tm` (в UTC) для преобразования.
/// \~english @return The `__time32_t` value corresponding to the input `struct tm`.
/// \~russian @return Значение `__time32_t`, соответствующее входной структуре `struct tm`.
#ifdef PLATFORM_CPU32
__time32_t mkgmtime(struct tm * const tm_);
#endif

/// \~english @brief Converts a `struct tm` (representing UTC) into a 64-bit `time_t` value on 64-bit Windows platforms.
///     This is a Windows-specific equivalent to the non-standard `timegm` function.
/// \~russian @brief Преобразует `struct tm` (представляющую UTC) в 64-битное значение `time_t` на 64-битных платформах Windows.
///     Это специфичный для Windows эквивалент нестандартной функции `timegm`.
/// \~english @param tm_ Pointer to the `struct tm` object (in UTC) to convert.
/// \~russian @param tm_ Указатель на объект `struct tm` (в UTC) для преобразования.
/// \~english @return The `__time64_t` value corresponding to the input `struct tm`.
/// \~russian @return Значение `__time64_t`, соответствующее входной структуре `struct tm`.
#ifdef PLATFORM_CPU64
__time64_t mkgmtime(struct tm * const tm_);
#endif

/// \~english @brief Template wrapper for the Windows C runtime function `sscanf_s`.
///     Provides a type-safe way to call `sscanf_s` with variadic arguments. `sscanf_s` is the secure version of `sscanf`.
/// \~russian @brief Шаблонная обертка для функции времени выполнения Windows C `sscanf_s`.
///     Предоставляет типобезопасный способ вызова `sscanf_s` с переменным числом аргументов. `sscanf_s` является безопасной версией `sscanf`.
/// \~english @tparam TArgs Argument types for `sscanf_s`.
/// \~russian @tparam TArgs Типы аргументов для `sscanf_s`.
/// \~english @param args Arguments to be passed to `::sscanf_s`.
/// \~russian @param args Аргументы для передачи в `::sscanf_s`.
/// \~english @return The value returned by `::sscanf_s`.
/// \~russian @return Значение, возвращаемое `::sscanf_s`.
template<typename ... TArgs>
int sscanf(TArgs ... args) {
    return ::sscanf_s(args ...);
}

/// \~english @brief Template wrapper for the C standard library function `snprintf`.
///     On modern MSVC, `snprintf` is C99 compliant and behaves like the secure version `_snprintf_s`.
///     Provides a type-safe way to call `snprintf` with variadic arguments.
/// \~russian @brief Шаблонная обертка для стандартной функции библиотеки C `snprintf`.
///     В современных версиях MSVC `snprintf` соответствует C99 и ведет себя как безопасная версия `_snprintf_s`.
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

/// \~english @brief Defines a secure sprintf-like function using a Windows SDK macro.
///     This macro likely expands to a template function that wraps `vsprintf_s` or a similar secure variant,
///     providing type safety and buffer overflow protection for string formatting.
/// \~russian @brief Определяет безопасную sprintf-подобную функцию с использованием макроса из Windows SDK.
///     Этот макрос, вероятно, раскрывается в шаблонную функцию, которая оборачивает `vsprintf_s` или аналогичный безопасный вариант,
///     обеспечивая типобезопасность и защиту от переполнения буфера при форматировании строк.
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, sprintf, vsprintf_s, _Post_z_ char, _Dest, _In_z_ _Printf_format_string_ const char *, _Format);


} // iridium::convertion::implementation::platform


#endif // WINDOWS_PLATFORM


#endif // HEADER_CONVERT_331F2175_E59F_4DD5_A043_451816549CD0
