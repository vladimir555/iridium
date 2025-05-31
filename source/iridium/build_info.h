// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3
#define HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3


#include "platform.h"

#include <cstdio>
#include <cstddef>
#include <cstring>

#ifdef UNIX_PLATFORM
#include <sys/utsname.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


/// \~english @brief Retrieves a string containing detailed build information.
///     This function collects various pieces of information about the build environment
///     at compile time. The information includes ABI, STL version, compiler details,
///     architecture, thread support, optimization level, C++ standard version,
///     libc version (for Unix-like systems), RTTI status, exception support status,
///     and platform information. The information is formatted into a human-readable string.
/// \~russian @brief Получает строку, содержащую подробную информацию о сборке.
///     Эта функция собирает различную информацию о среде сборки
///     во время компиляции. Информация включает ABI, версию STL, сведения о компиляторе,
///     архитектуру, поддержку потоков, уровень оптимизации, версию стандарта C++,
///     версию libc ( для Unix-подобных систем), статус RTTI, статус поддержки исключений
///     и информацию о платформе. Информация форматируется в человекочитаемую строку.
/// \~english @return A pointer to a static character buffer containing the build information.
///     The buffer is overwritten on each call in some contexts, so it should be used or copied immediately.
/// \~russian @return Указатель на статический символьный буфер, содержащий информацию о сборке.
///     Буфер может перезаписываться при каждом вызове в некоторых контекстах, поэтому его следует использовать или скопировать немедленно.
inline char const *getBuildInfoInline() {
    // Статический буфер
    static char buffer[2048] = {0};
    // Текущая позиция в буфере
    size_t offset = 0;

    // Проверка ABI
    #ifdef _GLIBCXX_USE_CXX11_ABI
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "abi: %d\n", _GLIBCXX_USE_CXX11_ABI);
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "abi: none\n");
    #endif

    // Проверка стандартной библиотеки
    #ifdef __GLIBCXX__
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: libstdc++/%d\n", __GLIBCXX__);
    #elif defined(_LIBCPP_VERSION)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: libc++/%d\n", _LIBCPP_VERSION);
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: unknown\n");
    #endif

    // Проверка версии компилятора
    #if defined(__clang__)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: clang/%d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #elif defined(__GNUC__)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: gcc/%d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: msvc/%d\n", _MSC_VER);
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: unknown\n");
    #endif

    // Проверка архитектуры
    #if defined(__x86_64__)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: x86_64\n");
    #elif defined(__aarch64__)
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: aarch64\n");
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: unknown\n");
    #endif

    // Проверка поддержки потоков
    #ifdef _REENTRANT
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "threads: true\n");
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "threads: false\n");
    #endif

    // Проверка уровня оптимизации
    #ifdef __OPTIMIZE__
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "optimization: O%d\n", __OPTIMIZE__);
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "optimization: none\n");
    #endif

    // Проверка стандарта C++
    #ifdef __cplusplus
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "std: %ld\n", __cplusplus);
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "std: unknown\n");
    #endif

#ifdef UNIX_PLATFORM
    // Проверка версии glibc
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "libc: glibc/%s\n", sys_info.release);
    } else {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "libc: unknown\n");
    }
#endif

    // Проверка RTTI
    #ifdef __GXX_RTTI
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "rtti: true\n");
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "rtti: false\n");
    #endif

    // Проверка исключений
    #ifdef __EXCEPTIONS
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "exceptions: true\n");
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "exceptions: false\n");
    #endif

    // Проверка платформы
    #ifdef PLATFORM_HEADER
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "platform: %s\n", PLATFORM_HEADER());
    #else
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "platform: unknown\n");
    #endif

    return buffer;
}


#ifdef __cplusplus
} // extern "C"
#endif


#endif // HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3
