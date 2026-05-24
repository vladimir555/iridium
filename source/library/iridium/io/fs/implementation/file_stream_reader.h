// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the `CFileStreamReader` class, a concrete implementation for read-only file streams.
/// \~russian @brief Определяет класс `CFileStreamReader`, конкретную реализацию для файловых потоков, предназначенных только для чтения.
#ifndef HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
#define HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371


#include "file_stream.h"       // For CFileStream base class
#include "iridium/smart_ptr.h" // For DEFINE_IMPLEMENTATION (if it's related to smart pointers or factories)
#include <string>              // For std::string


namespace iridium::io::fs::implementation {


/// \~english @brief A concrete file stream implementation specialized for reading.
///     This class derives from `CFileStream` and configures it for read-only operations.
///     It provides all the functionalities of `CFileStream` (which implements `IFileStreamReader`
///     and `IFileStreamWriter`), but its constructor ensures it's opened in a mode suitable for reading.
///     While it inherits write capabilities from `CFileStream`, its intended use is for reading.
/// \~russian @brief Конкретная реализация файлового потока, специализированная для чтения.
///     Этот класс наследуется от `CFileStream` и настраивает его для операций только для чтения.
///     Он предоставляет все функциональные возможности `CFileStream` (который реализует `IFileStreamReader`
///     и `IFileStreamWriter`), но его конструктор гарантирует, что он открывается в режиме, подходящем для чтения.
///     Хотя он наследует возможности записи от `CFileStream`, его предполагаемое использование - для чтения.
class CFileStreamReader: public virtual CFileStream { // Virtual inheritance consistent with CFileStream's bases
public:
    /// \~english @brief Macro for defining implementation details, possibly for type registration or factory systems.
    /// \~russian @brief Макрос для определения деталей реализации, возможно, для регистрации типов или фабричных систем.
    DEFINE_IMPLEMENTATION(CFileStreamReader)

    /// \~english @brief Constructs a `CFileStreamReader` to read from the specified file path.
    ///     Initializes the underlying `CFileStream` with the given path and sets the open mode to `TOpenMode::READ`.
    ///     The actual file opening and resource allocation occur during the `initialize()` call.
    /// \~russian @brief Конструирует `CFileStreamReader` для чтения из указанного пути к файлу.
    ///     Инициализирует базовый `CFileStream` указанным путем и устанавливает режим открытия `TOpenMode::READ`.
    ///     Фактическое открытие файла и выделение ресурсов происходят во время вызова `initialize()`.
    /// \~english @param path The path to the file to be opened for reading.
    /// \~russian @param path Путь к файлу, который должен быть открыт для чтения.
    explicit CFileStreamReader(std::string const &path);
};


} // iridium::io::fs::implementation


#endif // HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
