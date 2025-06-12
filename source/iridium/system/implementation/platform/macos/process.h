#ifndef HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
#define HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM

/// \~english @file
/// @brief macOS-specific process implementation header.
///     For the macOS platform, this file primarily delegates to or reuses common Unix
///     process functionalities, leveraging the Unix underpinnings of macOS.
/// \~russian @file
/// @brief Платформо-специфичный заголовочный файл реализации процессов для macOS.
///     Для платформы macOS этот файл в основном делегирует или повторно использует общие
///     функциональные возможности процессов Unix, используя Unix-основу macOS.

/// \~english @brief Includes the Unix process implementation header.
///     macOS, being a Unix-based system, utilizes the generic Unix implementation
///     provided by the project for its process handling.
/// \~russian @brief Включает заголовочный файл реализации процессов Unix.
///     macOS, будучи Unix-подобной системой, использует общую реализацию Unix,
///     предоставленную проектом, для обработки процессов.
#include "../unix/process.h"


namespace iridium {
namespace system {
namespace implementation {
/// \~english @brief Contains platform-specific implementations for system components, tailored for the macOS environment.
/// \~russian @brief Содержит платформо-специфичные реализации системных компонентов, адаптированные для среды macOS.
namespace platform {

/// \~english @typedef CProcessStream
/// @brief Type alias for the macOS platform, mapping `CProcessStream` to the Unix `CProcessStream` implementation.
///     This indicates that for macOS, the process stream functionalities (handling stdin, stdout, stderr for processes)
///     are provided by the existing Unix implementation (`platform::unix_::CProcessStream`).
/// \~russian @typedef CProcessStream
/// @brief Псевдоним типа для платформы macOS, отображающий `CProcessStream` на реализацию `CProcessStream` для Unix.
///     Это указывает на то, что для macOS функциональность потоков процесса (обработка stdin, stdout, stderr для процессов)
///     предоставляется существующей реализацией Unix (`platform::unix_::CProcessStream`).
typedef platform::unix_::CProcessStream CProcessStream;


} // platform
} // implementation
} // system
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
