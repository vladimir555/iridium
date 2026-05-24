/// \~english @brief macOS-specific header to include appropriate low-level file API implementations.
///     This file ensures that when compiling for the macOS platform, the necessary file system
///     operations are sourced, in this case, by reusing the FreeBSD file API implementation
///     due to their shared BSD heritage and POSIX compliance.
/// \~russian @brief Специфичный для macOS заголовочный файл для подключения соответствующих низкоуровневых реализаций файлового API.
///     Этот файл гарантирует, что при компиляции для платформы macOS подключаются необходимые
///     операции файловой системы, в данном случае, путем повторного использования реализации файлового API для FreeBSD
///     в силу их общего наследия BSD и совместимости с POSIX.
#ifndef HEADER_FILE_API_798CFB63_C4CB_4B9B_AA5E_6C35A52ECEC9
#define HEADER_FILE_API_798CFB63_C4CB_4B9B_AA5E_6C35A52ECEC9


#include "iridium/platform.h" // For platform detection, specifically MACOS_PLATFORM


/// \~english @brief Conditional compilation block for the macOS platform.
///     The definitions and includes within this block are only active when `MACOS_PLATFORM` is defined.
/// \~russian @brief Блок условной компиляции для платформы macOS.
///     Определения и подключения внутри этого блока активны только тогда, когда определен макрос `MACOS_PLATFORM`.
#ifdef MACOS_PLATFORM


/// \~english @brief Includes the FreeBSD-specific low-level file API implementation header.
///     For the macOS platform, Iridium's file system operations largely rely on or
///     reuse the implementations provided for FreeBSD, given their common architectural roots.
/// \~russian @brief Подключает заголовочный файл с низкоуровневыми реализациями файлового API для FreeBSD.
///     Для платформы macOS операции файловой системы Iridium в значительной степени полагаются
///     на реализации, предоставленные для FreeBSD, или повторно используют их, учитывая их общие архитектурные корни.
#include "../freebsd/file_api.h"


#endif // MACOS_PLATFORM


#endif // HEADER_FILE_API_798CFB63_C4CB_4B9B_AA5E_6C35A52ECEC9
