/// \~english @brief Platform-specific header for Emscripten to include appropriate low-level file API implementations.
///     This file ensures that when compiling for the Emscripten platform, the necessary file system
///     operations are sourced, in this case, by reusing the Linux file API implementation.
/// \~russian @brief Платформо-специфичный заголовочный файл для Emscripten для подключения соответствующих низкоуровневых реализаций файлового API.
///     Этот файл гарантирует, что при компиляции для платформы Emscripten подключаются необходимые
///     операции файловой системы, в данном случае, путем повторного использования реализации файлового API для Linux.
#ifndef HEADER_FILE_API_D63F8D8A_FEB6_40BF_B213_7D33EAF4C4BA
#define HEADER_FILE_API_D63F8D8A_FEB6_40BF_B213_7D33EAF4C4BA


#include "iridium/platform.h" // For platform detection, specifically EMSCRIPTEN_PLATFORM


/// \~english @brief Conditional compilation block for the Emscripten platform.
///     The definitions and includes within this block are only active when `EMSCRIPTEN_PLATFORM` is defined.
/// \~russian @brief Блок условной компиляции для платформы Emscripten.
///     Определения и подключения внутри этого блока активны только тогда, когда определен макрос `EMSCRIPTEN_PLATFORM`.
#ifdef EMSCRIPTEN_PLATFORM


/// \~english @brief Includes the Linux-specific low-level file API implementation header.
///     For the Emscripten platform, Iridium's file system operations largely rely on or
///     reuse the implementations provided for the Linux platform, leveraging Emscripten's
///     POSIX-like environment.
/// \~russian @brief Подключает заголовочный файл с низкоуровневыми реализациями файлового API для Linux.
///     Для платформы Emscripten операции файловой системы Iridium в значительной степени полагаются
///     на реализации, предоставленные для платформы Linux, или повторно используют их,
///     используя POSIX-подобное окружение Emscripten.
#include "../linux/file_api.h"


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_FILE_API_D63F8D8A_FEB6_40BF_B213_7D33EAF4C4BA
