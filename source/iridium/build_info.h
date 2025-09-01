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


inline char const *getBuildInfoInline() {
    // Static buffer to hold the build info string
    static char buffer[2048] = {0};

    // Current write position in the buffer
    size_t offset = 0;

    // Lambda to safely append formatted strings to the buffer
    // Checks snprintf return value and prevents buffer overflow
    auto safe_append = [&](const char *format, ...) -> void {
        // Do nothing if already at or beyond buffer capacity
        if (offset >= sizeof(buffer)) {
            return;
        }

        va_list args;
        va_list args_copy;
        va_start(args, format);
        va_copy(args_copy, args); // Required before calling vsnprintf

        // Attempt to write into the remaining space
        int written = vsnprintf(buffer + offset, sizeof(buffer) - offset, format, args);
        va_end(args);

        if (written > 0) {
            // If the output was truncated
            if (static_cast<size_t>(written) >= sizeof(buffer) - offset) {
                // Mark the end of the buffer (ensure null termination)
                offset = sizeof(buffer) - 1;
            } else {
                // Otherwise, advance the offset by the number of characters written
                offset += written;
            }
        }
        // If written <= 0, ignore (invalid format or empty output — unlikely in this context)
        va_end(args_copy);
    };

    // Check C++ ABI setting (libstdc++)
#ifdef _GLIBCXX_USE_CXX11_ABI
    safe_append("abi: %d\n", _GLIBCXX_USE_CXX11_ABI);
#else
    safe_append("abi: none\n");
#endif

    // Identify the standard library in use
#ifdef __GLIBCXX__
    safe_append("stl: libstdc++/%d\n", __GLIBCXX__);
#elif defined(_LIBCPP_VERSION)
    safe_append("stl: libc++/%d\n", _LIBCPP_VERSION);
#else
    safe_append("stl: unknown\n");
#endif

    // Compiler identification and version
#if defined(__clang__)
    safe_append("compiler: clang/%d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    safe_append("compiler: gcc/%d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    safe_append("compiler: msvc/%d\n", _MSC_VER);
#else
    safe_append("compiler: unknown\n");
#endif

    // CPU architecture detection
#if defined(__x86_64__)
    safe_append("arch: x86_64\n");
#elif defined(__aarch64__)
    safe_append("arch: aarch64\n");
#else
    safe_append("arch: unknown\n");
#endif

    // Thread support detection
#ifdef _REENTRANT
    safe_append("threads: true\n");
#else
    safe_append("threads: false\n");
#endif

    // Optimization level
#ifdef __OPTIMIZE__
    safe_append("optimization: O%d\n", __OPTIMIZE__);
#else
    safe_append("optimization: none\n");
#endif

    // C++ standard version
#ifdef __cplusplus
    safe_append("std: %ld\n", __cplusplus);
#else
    safe_append("std: unknown\n");
#endif

    // System libc version (only on Unix platforms)
#ifdef UNIX_PLATFORM
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        safe_append("libc: glibc/%s\n", sys_info.release);
    } else {
        safe_append("libc: unknown\n");
    }
#endif

    // RTTI (Run-Time Type Information) status
#ifdef __GXX_RTTI
    safe_append("rtti: true\n");
#else
    safe_append("rtti: false\n");
#endif

    // Exception handling support
#ifdef __EXCEPTIONS
    safe_append("exceptions: true\n");
#else
    safe_append("exceptions: false\n");
#endif

    // Platform name via optional macro
#ifdef PLATFORM_HEADER
    safe_append("platform: %s\n", PLATFORM_HEADER());
#else
    safe_append("platform: unknown\n");
#endif

    // Ensure null termination (defensive)
    buffer[sizeof(buffer) - 1] = '\0';

    // Return the filled buffer
    return buffer;
}


#ifdef __cplusplus
} // extern "C"
#endif


#endif // HEADER_BUILD_INFO_43DEDE80_1EBB_418D_AAE6_70EF2170AFE3
