/// \~english @brief Platform-agnostic include dispatcher for low-level file API implementations.
///     This header uses a macro (`PLATFORM_HEADER`) to include the appropriate platform-specific
///     `file_api.h` header (e.g., for Unix, Windows), which contains the actual declarations
///     of low-level file system operations.
/// \~russian @brief Платформо-независимый диспетчер подключения для низкоуровневых реализаций файлового API.
///     Этот заголовочный файл использует макрос (`PLATFORM_HEADER`) для подключения соответствующего
///     платформо-специфичного заголовочного файла `file_api.h` (например, для Unix, Windows),
///     который содержит фактические объявления низкоуровневых операций файловой системы.
#ifndef HEADER_FILE_API_9D774EEE_1199_44EA_8DFF_DB7440BF6CB2
#define HEADER_FILE_API_9D774EEE_1199_44EA_8DFF_DB7440BF6CB2


#include "iridium/platform.h" // For platform detection and PLATFORM_HEADER macro

/// \~english @brief Includes the platform-specific header file containing actual file API declarations.
///     The `PLATFORM_HEADER` macro resolves to the path of the `file_api.h` file
///     located in the subdirectory corresponding to the current target platform
///     (e.g., `iridium/io/fs/implementation/platform/unix/file_api.h` or
///     `iridium/io/fs/implementation/platform/windows/file_api.h`).
/// \~russian @brief Подключает платформо-специфичный заголовочный файл, содержащий фактические объявления файлового API.
///     Макрос `PLATFORM_HEADER` разрешается в путь к файлу `file_api.h`,
///     находящемуся в подкаталоге, соответствующем текущей целевой платформе
///     (например, `iridium/io/fs/implementation/platform/unix/file_api.h` или
///     `iridium/io/fs/implementation/platform/windows/file_api.h`).
#include PLATFORM_HEADER(file_api.h)


namespace iridium {
namespace io {
namespace fs {
/// \~english @brief Namespace for internal implementation details of file system functionalities.
/// \~russian @brief Пространство имен для внутренних деталей реализации функциональностей файловой системы.
namespace implementation {


/// \~english @brief Brings symbols from the platform-specific `file_api.h` (which are in the nested `platform` namespace)
///     into the current `iridium::io::fs::implementation` namespace.
///     This makes functions like `open`, `read`, `write`, `close`, etc., defined in the
///     platform-specific header, directly accessible within this namespace.
/// \~russian @brief Переносит символы из платформо-специфичного `file_api.h` (которые находятся во вложенном пространстве имен `platform`)
///     в текущее пространство имен `iridium::io::fs::implementation`.
///     Это делает функции, такие как `open`, `read`, `write`, `close` и т.д., определенные в
///     платформо-специфичном заголовочном файле, напрямую доступными в этом пространстве имен.
using namespace platform;


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_API_9D774EEE_1199_44EA_8DFF_DB7440BF6CB2
