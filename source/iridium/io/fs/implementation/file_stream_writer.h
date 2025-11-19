// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the `CFileStreamWriter` class, a concrete implementation for write-oriented file streams.
/// \~russian @brief Определяет класс `CFileStreamWriter`, конкретную реализацию для файловых потоков, ориентированных на запись.
#ifndef HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
#define HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3


#include "file_stream.h"       // For CFileStream base class
#include "iridium/smart_ptr.h" // For DEFINE_IMPLEMENTATION (if it's related to smart pointers or factories)
#include <string>              // For std::string


namespace iridium::io::fs::implementation {


class CFileStreamWriter: public virtual CFileStream {
public:
    /// \~english @brief Macro for defining implementation details, possibly for type registration or factory systems.
    /// \~russian @brief Макрос для определения деталей реализации, возможно, для регистрации типов или фабричных систем.
    DEFINE_IMPLEMENTATION(CFileStreamWriter)

    /// \~english @brief Constructs a `CFileStreamWriter` to write to the specified file path.
    ///     Initializes the underlying `CFileStream` with the given path and an open mode determined by `is_rewrite`.
    ///     The actual file opening and resource allocation occur during the `initialize()` call.
    /// \~russian @brief Конструирует `CFileStreamWriter` для записи в указанный путь к файлу.
    ///     Инициализирует базовый `CFileStream` указанным путем и режимом открытия, определяемым `is_rewrite`.
    ///     Фактическое открытие файла и выделение ресурсов происходят во время вызова `initialize()`.
    /// \~english @param path The path to the file to be opened for writing.
    /// \~russian @param path Путь к файлу, который должен быть открыт для записи.
    /// \~english @param is_rewrite Determines the open mode.
    ///     If `true`, the stream is initialized with `CFileStream::TOpenMode::REWRITE`, meaning the file will be created if it doesn't exist, or truncated to zero length if it does exist.
    ///     If `false` (default), the stream is initialized with `CFileStream::TOpenMode::WRITE`, which also creates the file if it doesn't exist or truncates it if it does.
    ///     Note: In the current `CFileStream` enum, both `WRITE` and `REWRITE` imply truncation/creation. If append behavior is needed, a different mode (e.g., `APPEND`) would be required in `CFileStream::TOpenMode`.
    /// \~russian @param is_rewrite Определяет режим открытия.
    ///     Если `true`, поток инициализируется с `CFileStream::TOpenMode::REWRITE`, что означает, что файл будет создан, если не существует, или усечен до нулевой длины, если существует.
    ///     Если `false` (по умолчанию), поток инициализируется с `CFileStream::TOpenMode::WRITE`, который также создает файл, если он не существует, или усекает его, если существует.
    ///     Примечание: В текущем перечислении `CFileStream::TOpenMode` и `WRITE`, и `REWRITE` подразумевают усечение/создание. Если необходимо поведение добавления, потребуется другой режим (например, `APPEND`) в `CFileStream::TOpenMode`.
    explicit CFileStreamWriter(std::string const &path, bool const &is_rewrite = false);
};


} // iridium::io::fs::implementation


#endif // HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
