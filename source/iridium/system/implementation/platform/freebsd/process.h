#ifndef HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
#define HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM

/// \~english @file
/// @brief FreeBSD-specific process implementation header.
///     For the FreeBSD platform, this file typically delegates to or reuses common Unix
///     process functionalities where applicable, as FreeBSD shares many Unix-like behaviors.
///     Note: There is a preprocessor directive mismatch in this file; it opens with `#ifdef FREEBSD_PLATFORM`
///     but closes with `#endif // LINUX_PLATFORM`. This should ideally be corrected to `#endif // FREEBSD_PLATFORM`.
/// \~russian @file
/// @brief Платформо-специфичный заголовочный файл реализации процессов для FreeBSD.
///     Для платформы FreeBSD этот файл обычно делегирует или повторно использует общие
///     функциональные возможности процессов Unix, где это применимо, поскольку FreeBSD разделяет
///     многие Unix-подобные поведения.
///     Примечание: В этом файле присутствует несоответствие директив препроцессора; он открывается
///     с `#ifdef FREEBSD_PLATFORM`, но закрывается с `#endif // LINUX_PLATFORM`. В идеале это должно
///     быть исправлено на `#endif // FREEBSD_PLATFORM`.

/// \~english @brief Includes the Unix process implementation header.
///     FreeBSD, being a Unix-like system, bases its process handling on the generic
///     Unix implementation provided by the project.
/// \~russian @brief Включает заголовочный файл реализации процессов Unix.
///     FreeBSD, будучи Unix-подобной системой, основывает свою обработку процессов на общей
///     реализации Unix, предоставленной проектом.
#include "../unix/process.h"


namespace iridium {
namespace system {
namespace implementation {
/// \~english @brief Contains platform-specific implementations for system components, tailored for the FreeBSD environment.
/// \~russian @brief Содержит платформо-специфичные реализации системных компонентов, адаптированные для среды FreeBSD.
namespace platform {

/// \~english @typedef CProcessStream
/// @brief Type alias for the FreeBSD platform, mapping `CProcessStream` to the Unix `CProcessStream` implementation.
///     This indicates that for FreeBSD, the process stream functionalities (handling stdin, stdout, stderr for processes)
///     are provided by the existing Unix implementation (`platform::unix_::CProcessStream`).
/// \~russian @typedef CProcessStream
/// @brief Псевдоним типа для платформы FreeBSD, отображающий `CProcessStream` на реализацию `CProcessStream` для Unix.
///     Это указывает на то, что для FreeBSD функциональность потоков процесса (обработка stdin, stdout, stderr для процессов)
///     предоставляется существующей реализацией Unix (`platform::unix_::CProcessStream`).
typedef platform::unix_::CProcessStream CProcessStream;


} // platform
} // implementation
} // system
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
