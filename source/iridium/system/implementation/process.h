// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0
#define HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0


#include "iridium/platform.h"

/// \~english @brief Includes the platform-specific process implementation header.
///     The actual file included (e.g., "platform/linux/process.h", "platform/windows/process.h")
///     is determined by the `PLATFORM_HEADER` macro, which resolves based on the detected operating system
///     and architecture during compilation. This header is expected to define `platform::CProcessStream`.
/// \~russian @brief Включает заголовочный файл с платформо-специфичной реализацией процесса.
///     Фактически включаемый файл (например, "platform/linux/process.h", "platform/windows/process.h")
///     определяется макросом `PLATFORM_HEADER`, который разрешается на основе обнаруженной операционной системы
///     и архитектуры во время компиляции. Ожидается, что этот заголовочный файл определит `platform::CProcessStream`.
#include PLATFORM_HEADER(process.h)


namespace iridium::system::implementation {

/// \~english @typedef CProcessStream
/// @brief Type alias for the platform-specific process stream implementation, which is expected to be `platform::CProcessStream`.
///     This class, defined in the platform-specific header included above, is responsible for
///     implementing the `iridium::io::IStreamPort` interface to provide access to the
///     standard input, output, and error streams of a system process. It also likely implements
///     the `iridium::system::IProcess` interface for process state management.
/// \~russian @typedef CProcessStream
/// @brief Псевдоним типа для платформо-специфичной реализации потоков процесса, которая, как ожидается, будет `platform::CProcessStream`.
///     Этот класс, определенный в платформо-специфичном заголовочном файле, включенном выше, отвечает за
///     реализацию интерфейса `iridium::io::IStreamPort` для предоставления доступа к
///     стандартным потокам ввода, вывода и ошибок системного процесса. Вероятно, он также реализует
///     интерфейс `iridium::system::IProcess` для управления состоянием процесса.
typedef platform::CProcessStream CProcessStream;


} // iridium::system::implementation


#endif // HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0
