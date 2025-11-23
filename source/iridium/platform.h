// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines macros for platform, compiler, and architecture detection, and for platform-specific path construction.
/// \~russian @file
/// @brief Определяет макросы для определения платформы, компилятора и архитектуры, а также для построения путей, специфичных для платформы.

#ifndef HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13
#define HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13


#ifdef __linux
#ifndef ANDROID
/// \~english @def LINUX_PLATFORM
/// @brief Defined when the target platform is Linux (excluding Android).
/// \~russian @def LINUX_PLATFORM
/// @brief Определяется, если целевая платформа - Linux (исключая Android).
#define LINUX_PLATFORM
#undef linux
#undef unix
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. Value varies by platform. For Linux, it's `platform/linux/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Значение зависит от платформы. Для Linux это `platform/linux/`.
#define PLATFORM_PATH platform/linux/
#endif
#endif


#ifdef ANDROID
/// \~english @def ANDROID_PLATFORM
/// @brief Defined when the target platform is Android.
/// \~russian @def ANDROID_PLATFORM
/// @brief Определяется, если целевая платформа - Android.
#define ANDROID_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For Android, it's `platform/android/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для Android это `platform/android/`.
#define PLATFORM_PATH platform/android/
#endif


#ifdef _WIN32
/// \~english @def WINDOWS_PLATFORM
/// @brief Defined when the target platform is Windows.
/// \~russian @def WINDOWS_PLATFORM
/// @brief Определяется, если целевая платформа - Windows.
#define WINDOWS_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For Windows, it's `platform/windows/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для Windows это `platform/windows/`.
#define PLATFORM_PATH platform/windows/
#if _WIN32 || _WIN64
#if _WIN64
#define CPU64
#else
#define CPU32
#endif
#endif
#endif


#ifdef __EMSCRIPTEN__
/// \~english @def EMSCRIPTEN_PLATFORM
/// @brief Defined when the target platform is Emscripten (WebAssembly).
/// \~russian @def EMSCRIPTEN_PLATFORM
/// @brief Определяется, если целевая платформа - Emscripten (WebAssembly).
#define EMSCRIPTEN_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For Emscripten, it's `platform/emscripten/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для Emscripten это `platform/emscripten/`.
#define PLATFORM_PATH platform/emscripten/
#endif


#ifdef __APPLE_OS__
/// \~english @def MACOS_PLATFORM
/// @brief Defined when the target platform is macOS. This check is often more specific than just `__APPLE__`.
/// \~russian @def MACOS_PLATFORM
/// @brief Определяется, если целевая платформа - macOS. Эта проверка часто более специфична, чем просто `__APPLE__`.
#define MACOS_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For macOS, it's `platform/macos/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для macOS это `platform/macos/`.
#define PLATFORM_PATH platform/macos/
#endif


#ifdef __APPLE__
// This is a more general Apple platform check, MACOS_PLATFORM is preferred for macOS specific code.
#ifndef MACOS_PLATFORM // Ensure MACOS_PLATFORM and its PLATFORM_PATH take precedence if __APPLE_OS__ was also defined.
/// \~english @def MACOS_PLATFORM
/// @brief Defined when the target platform is Apple (macOS or iOS). If `__APPLE_OS__` is also defined, this definition might be redundant or a fallback.
/// \~russian @def MACOS_PLATFORM
/// @brief Определяется, если целевая платформа - Apple (macOS или iOS). Если также определен `__APPLE_OS__`, это определение может быть избыточным или запасным вариантом.
#define MACOS_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For Apple platforms (macOS/iOS), it's `platform/macos/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для платформ Apple (macOS/iOS) это `platform/macos/`.
#define PLATFORM_PATH platform/macos/
#endif
#endif


#ifdef __FreeBSD__
/// \~english @def FREEBSD_PLATFORM
/// @brief Defined when the target platform is FreeBSD.
/// \~russian @def FREEBSD_PLATFORM
/// @brief Определяется, если целевая платформа - FreeBSD.
#define FREEBSD_PLATFORM
/// \~english @def PLATFORM_PATH
/// @brief Defines the base path for platform-specific headers. For FreeBSD, it's `platform/freebsd/`.
/// \~russian @def PLATFORM_PATH
/// @brief Определяет базовый путь для заголовков, специфичных для платформы. Для FreeBSD это `platform/freebsd/`.
#define PLATFORM_PATH platform/freebsd/
#endif


#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)
/// \~english @def UNIX_PLATFORM
/// @brief Defined if the target platform is a Unix-like system (Linux, FreeBSD, macOS, Emscripten).
/// \~russian @def UNIX_PLATFORM
/// @brief Определяется, если целевая платформа является Unix-подобной системой (Linux, FreeBSD, macOS, Emscripten).
#define UNIX_PLATFORM
#endif


#if defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)
/// \~english @def FREEBSD_LIKE_PLATFORM
/// @brief Defined if the target platform is FreeBSD or macOS, indicating similarities in system APIs (e.g., kqueue).
/// \~russian @def FREEBSD_LIKE_PLATFORM
/// @brief Определяется, если целевая платформа - FreeBSD или macOS, что указывает на сходство в системных API (например, kqueue).
#define FREEBSD_LIKE_PLATFORM
#endif


/// \~english @def PLATFORM_IDENT(x)
/// @brief An identity macro that returns its argument unchanged. Used for macro expansion.
/// \~russian @def PLATFORM_IDENT(x)
/// @brief Макрос-идентификатор, который возвращает свой аргумент без изменений. Используется для расширения макросов.
#define PLATFORM_IDENT(x) x
/// \~english @def PLATFORM_XSTR(x)
/// @brief A helper macro that stringifies its argument.
/// \~russian @def PLATFORM_XSTR(x)
/// @brief Вспомогательный макрос, который превращает свой аргумент в строку.
#define PLATFORM_XSTR(x) #x
/// \~english @def PLATFORM_STR(x)
/// @brief A helper macro that stringifies its argument after expansion.
/// \~russian @def PLATFORM_STR(x)
/// @brief Вспомогательный макрос, который превращает свой аргумент в строку после расширения.
#define PLATFORM_STR(x) PLATFORM_XSTR(x)
/// \~english @def PLATFORM_HEADER(file_name)
/// @brief Constructs a string literal for including a platform-specific header file.
/// It combines `PLATFORM_PATH` with the provided `file_name`.
/// Example: `PLATFORM_HEADER(my_header.h)` might become `"platform/linux/my_header.h"`.
/// \~russian @def PLATFORM_HEADER(file_name)
/// @brief Создает строковый литерал для включения заголовочного файла, специфичного для платформы.
/// Он объединяет `PLATFORM_PATH` с предоставленным `file_name`.
/// Пример: `PLATFORM_HEADER(my_header.h)` может превратиться в `"platform/linux/my_header.h"`.
#define PLATFORM_HEADER(file_name) PLATFORM_STR(PLATFORM_IDENT(PLATFORM_PATH)PLATFORM_IDENT(file_name))


#if _WIN32 || _WIN64
    #if _WIN64
        /// \~english @def PLATFORM_CPU64
        /// @brief Defined if the CPU architecture is 64-bit. This specific instance is for Windows.
        /// \~russian @def PLATFORM_CPU64
        /// @brief Определяется, если архитектура ЦП - 64-битная. Этот конкретный экземпляр предназначен для Windows.
        #define PLATFORM_CPU64
    #else
        /// \~english @def PLATFORM_CPU32
        /// @brief Defined if the CPU architecture is 32-bit. This specific instance is for Windows.
        /// \~russian @def PLATFORM_CPU32
        /// @brief Определяется, если архитектура ЦП - 32-битная. Этот конкретный экземпляр предназначен для Windows.
        #define PLATFORM_CPU32
    #endif
#endif


// Check GCC
#if __GNUC__
    #if defined(__x86_64__) || defined(__ppc64__) || defined (__arm64__)
        #ifndef PLATFORM_CPU64 // Avoid redefinition if already set by Windows checks (though unlikely for __GNUC__)
        /// \~english @def PLATFORM_CPU64
        /// @brief Defined if the CPU architecture is 64-bit, detected under GCC.
        /// \~russian @def PLATFORM_CPU64
        /// @brief Определяется, если архитектура ЦП - 64-битная, обнаружено под GCC.
        #define PLATFORM_CPU64
        #endif
    #else
        #ifndef PLATFORM_CPU32 // Avoid redefinition
        /// \~english @def PLATFORM_CPU32
        /// @brief Defined if the CPU architecture is 32-bit, detected under GCC.
        /// \~russian @def PLATFORM_CPU32
        /// @brief Определяется, если архитектура ЦП - 32-битная, обнаружено под GCC.
        #define PLATFORM_CPU32
        #endif
    #endif
#endif


// Visual Studio
#if defined(_MSC_VER)
#ifdef NDEBUG
#else
/// \~english @def BUILD_TYPE_DEBUG
/// @brief Defined if the build type is Debug (NDEBUG is not defined). This specific instance is for MSVC.
/// \~russian @def BUILD_TYPE_DEBUG
/// @brief Определяется, если тип сборки - Debug (NDEBUG не определен). Этот конкретный экземпляр предназначен для MSVC.
#define BUILD_TYPE_DEBUG
#endif
// GCC and Clang
#elif defined(__GNUC__) || defined(__clang__)
#ifdef NDEBUG
#else
/// \~english @def BUILD_TYPE_DEBUG
/// @brief Defined if the build type is Debug (NDEBUG is not defined). This specific instance is for GCC/Clang.
/// \~russian @def BUILD_TYPE_DEBUG
/// @brief Определяется, если тип сборки - Debug (NDEBUG не определен). Этот конкретный экземпляр предназначен для GCC/Clang.
#define BUILD_TYPE_DEBUG
#endif
// Unknown compiler
#else
// #define BUILD_TYPE_UNKNOWN
#endif


#endif // HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13
