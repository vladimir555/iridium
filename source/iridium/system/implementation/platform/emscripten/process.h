#ifndef HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29
#define HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29


#include "iridium/platform.h"


#ifdef EMSCRIPTEN_PLATFORM

/// \~english @file
/// @brief Emscripten-specific process implementation header.
///     For the Emscripten platform, which compiles C++ to WebAssembly for execution in web browsers,
///     true process creation and management capabilities are limited. This implementation typically
///     delegates to or reuses Unix-like process functionalities where applicable and feasible
///     within the Emscripten environment.
/// \~russian @file
/// @brief Платформо-специфичный заголовочный файл реализации процессов для Emscripten.
///     Для платформы Emscripten, которая компилирует C++ в WebAssembly для выполнения в веб-браузерах,
///     возможности создания и управления процессами ограничены. Эта реализация обычно
///     делегирует или повторно использует функциональность Unix-подобных процессов, где это применимо
///     и возможно в среде Emscripten.

/// \~english @brief Includes the Unix process implementation header.
///     Emscripten often emulates a Unix-like environment, so its process handling
///     can be based on the generic Unix implementation provided by the project.
/// \~russian @brief Включает заголовочный файл реализации процессов Unix.
///     Emscripten часто эмулирует Unix-подобную среду, поэтому его обработка процессов
///     может быть основана на общей реализации Unix, предоставленной проектом.
#include "../unix/process.h"


namespace iridium {
namespace system {
namespace implementation {
/// \~english @brief Contains platform-specific implementations for system components, tailored for the Emscripten environment.
/// \~russian @brief Содержит платформо-специфичные реализации системных компонентов, адаптированные для среды Emscripten.
namespace platform {

/// \~english @typedef CProcessStream
/// @brief Type alias for the Emscripten platform, mapping `CProcessStream` to the Unix `CProcessStream` implementation.
///     This indicates that for Emscripten, the process stream functionalities (handling stdin, stdout, stderr for processes)
///     are provided by the existing Unix implementation (`platform::unix_::CProcessStream`).
/// \~russian @typedef CProcessStream
/// @brief Псевдоним типа для платформы Emscripten, отображающий `CProcessStream` на реализацию `CProcessStream` для Unix.
///     Это указывает на то, что для Emscripten функциональность потоков процесса (обработка stdin, stdout, stderr для процессов)
///     предоставляется существующей реализацией Unix (`platform::unix_::CProcessStream`).
typedef platform::unix_::CProcessStream CProcessStream;


} // platform
} // implementation
} // system
} // iridium


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29
