#ifndef HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
#define HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM

/// \~english @file
/// @brief Linux-specific process implementation header.
///     For the Linux platform, this file primarily delegates to or reuses common Unix
///     process functionalities, as Linux is a Unix-like system.
/// \~russian @file
/// @brief Платформо-специфичный заголовочный файл реализации процессов для Linux.
///     Для платформы Linux этот файл в основном делегирует или повторно использует общие
///     функциональные возможности процессов Unix, поскольку Linux является Unix-подобной системой.

/// \~english @brief Includes the Unix process implementation header.
///     Linux, being a Unix-like system, bases its process handling on the generic
///     Unix implementation provided by the project.
/// \~russian @brief Включает заголовочный файл реализации процессов Unix.
///     Linux, будучи Unix-подобной системой, основывает свою обработку процессов на общей
///     реализации Unix, предоставленной проектом.
#include "../unix/process.h"


namespace iridium {
namespace system {
namespace implementation {
/// \~english @brief Contains platform-specific implementations for system components, tailored for the Linux environment.
/// \~russian @brief Содержит платформо-специфичные реализации системных компонентов, адаптированные для среды Linux.
namespace platform {

/// \~english @typedef CProcessStream
/// @brief Type alias for the Linux platform, mapping `CProcessStream` to the Unix `CProcessStream` implementation.
///     This indicates that for Linux, the process stream functionalities (handling stdin, stdout, stderr for processes)
///     are provided by the existing Unix implementation (`platform::unix_::CProcessStream`).
/// \~russian @typedef CProcessStream
/// @brief Псевдоним типа для платформы Linux, отображающий `CProcessStream` на реализацию `CProcessStream` для Unix.
///     Это указывает на то, что для Linux функциональность потоков процесса (обработка stdin, stdout, stderr для процессов)
///     предоставляется существующей реализацией Unix (`platform::unix_::CProcessStream`).
typedef platform::unix_::CProcessStream CProcessStream;


} // platform
} // implementation
} // system
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
