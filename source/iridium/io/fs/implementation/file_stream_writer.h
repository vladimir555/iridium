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


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


/// \~english @brief A concrete file stream implementation specialized for writing.
///     This class derives from `CFileStream` and configures it for write operations.
///     It provides all the functionalities of `CFileStream` (which implements `IFileStreamReader`
///     and `IFileStreamWriter`), but its constructor allows specifying whether to rewrite (truncate)
///     the file or use a default write mode (which might append or truncate based on `CFileStream::TOpenMode::WRITE` behavior).
/// \~russian @brief Конкретная реализация файлового потока, специализированная для записи.
///     Этот класс наследуется от `CFileStream` и настраивает его для операций записи.
///     Он предоставляет все функциональные возможности `CFileStream` (который реализует `IFileStreamReader`
///     и `IFileStreamWriter`), но его конструктор позволяет указать, следует ли перезаписывать (усекать)
///     файл или использовать режим записи по умолчанию (который может добавлять или усекать в зависимости от поведения `CFileStream::TOpenMode::WRITE`).
class CFileStreamWriter: public virtual CFileStream { // Virtual inheritance consistent with CFileStream's bases
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
    /// \~english @param is_rewrite If `true`, the file is opened in a mode that truncates it if it exists (typically `CFileStream::TOpenMode::REWRITE`).
    ///     If `false` (default), the file is opened in the default write mode (`CFileStream::TOpenMode::WRITE`), which might append or truncate
    ///     depending on the specific implementation of `CFileStream::initialize()` for that mode.
    /// \~russian @param is_rewrite Если `true`, файл открывается в режиме, который усекает его, если он существует (обычно `CFileStream::TOpenMode::REWRITE`).
    ///     Если `false` (по умолчанию), файл открывается в режиме записи по умолчанию (`CFileStream::TOpenMode::WRITE`), который может добавлять или усекать
    ///     в зависимости от конкретной реализации `CFileStream::initialize()` для этого режима.
    explicit CFileStreamWriter(std::string const &path, bool const &is_rewrite = false);
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
