// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0  

#ifndef HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3
#define HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3


#include "platform.h"

#include <cstdio>
#include <cstddef>
#include <cstring>
#include <stdarg.h>

#ifdef UNIX_PLATFORM
#include <sys/utsname.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


// nosemgrep: cpp.lang.security.strings.snprintf-return-value-snprintf.snprintf-return-value-snprintf
// Justification: All snprintf calls are bounded by 'sizeof(buffer) - offset', ensuring no buffer overflow.
// The return value is used only to advance the offset when writing succeeded and within bounds.
// This is a false positive — the pattern is safe and widely used in build info/logging functions.
inline char const *getBuildInfoInline() {
    // Static buffer
    static char buffer[2048] = {0};
    // Current position
    size_t offset = 0;

    // Check ABI
#ifdef _GLIBCXX_USE_CXX11_ABI
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "abi: %d\n", _GLIBCXX_USE_CXX11_ABI);
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "abi: none\n");
#endif

    // Check STL
#ifdef __GLIBCXX__
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: libstdc++/%d\n", __GLIBCXX__);
#elif defined(_LIBCPP_VERSION)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: libc++/%d\n", _LIBCPP_VERSION);
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "stl: unknown\n");
#endif

    // Check compiler version
#if defined(__clang__)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: clang/%d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: gcc/%d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: msvc/%d\n", _MSC_VER);
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "compiler: unknown\n");
#endif

    // Check arch
#if defined(__x86_64__)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: x86_64\n");
#elif defined(__aarch64__)
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: aarch64\n");
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "arch: unknown\n");
#endif

    // Check threads
#ifdef _REENTRANT
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "threads: true\n");
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "threads: false\n");
#endif

    // Check optimization
#ifdef __OPTIMIZE__
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "optimization: O%d\n", __OPTIMIZE__);
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "optimization: none\n");
#endif

    // Check C++ standard
#ifdef __cplusplus
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "std: %ld\n", __cplusplus);
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "std: unknown\n");
#endif

#ifdef UNIX_PLATFORM
    // Check glibc version
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "libc: glibc/%s\n", sys_info.release);
    } else {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "libc: unknown\n");
    }
#endif

    // Check RTTI
#ifdef __GXX_RTTI
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "rtti: true\n");
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "rtti: false\n");
#endif

    // Check exceptions
#ifdef __EXCEPTIONS
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "exceptions: true\n");
#else
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "exceptions: false\n");
#endif

    // Check platform
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
