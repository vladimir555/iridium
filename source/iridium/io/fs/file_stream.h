// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines interfaces for file stream operations, including reading, writing, and retrieving file status.
/// \~russian @brief Определяет интерфейсы для операций с файловыми потоками, включая чтение, запись и получение статуса файла.
#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "iridium/io/stream.h" // For IStream, IStreamReader, IStreamWriter
#include "iridium/smart_ptr.h" // For DEFINE_INTERFACE

#include <chrono>   // For std::chrono::system_clock::time_point
#include <cstddef>  // For size_t


namespace iridium {
namespace io {
namespace fs {


/// \~english @brief Structure holding status information about a file.
///     This includes the last modification time and the size of the file.
/// \~russian @brief Структура, содержащая информацию о состоянии файла.
///     Включает время последнего изменения и размер файла.
struct TFileStatus {
    /// \~english @brief The time point of the last modification of the file.
    /// \~russian @brief Момент времени последнего изменения файла.
    std::chrono::system_clock::time_point
        last_modified;
    /// \~english @brief The size of the file in bytes.
    /// \~russian @brief Размер файла в байтах.
    size_t
        size;
};


/// \~english @brief Base interface for file-specific streams.
///     Extends the generic `IStream` interface with file-specific operations like retrieving file status.
///     Uses virtual inheritance to support multiple inheritance in combined interfaces like `IFileStreamWriter` or `IFileStreamReader`.
/// \~russian @brief Базовый интерфейс для файловых потоков.
///     Расширяет общий интерфейс `IStream` специфичными для файлов операциями, такими как получение статуса файла.
///     Использует виртуальное наследование для поддержки множественного наследования в комбинированных интерфейсах, таких как `IFileStreamWriter` или `IFileStreamReader`.
class IFileStream: public virtual IStream {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IFileStream)

    /// \~english @brief Retrieves the status information of the file associated with this stream.
    /// \~russian @brief Получает информацию о состоянии файла, связанного с этим потоком.
    /// \~english @return A `TFileStatus` structure containing details like last modification time and file size.
    /// \~russian @return Структура `TFileStatus`, содержащая такие детали, как время последнего изменения и размер файла.
    /// \~english @throws iridium::Exception or derived classes if the file status cannot be retrieved.
    /// \~russian @throws iridium::Exception или производные классы, если статус файла не может быть получен.
    virtual TFileStatus getStatus() const = 0;
};


/// \~english @brief Interface for writable file streams.
///     Combines file-specific stream properties from `IFileStream` with generic stream writing capabilities from `IStreamWriter`.
/// \~russian @brief Интерфейс для записываемых файловых потоков.
///     Объединяет специфичные для файлов свойства потока из `IFileStream` с общими возможностями записи потока из `IStreamWriter`.
class IFileStreamWriter: public virtual IFileStream, public virtual IStreamWriter {
public:
    /// \~english @brief Macro used to define common interface elements.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    DEFINE_INTERFACE(IFileStreamWriter)

    /// \~english @brief Flushes any buffered data to the underlying file.
    ///     Ensures that all data written to the stream via `write()` is physically persisted to the file.
    /// \~russian @brief Сбрасывает все буферизованные данные в базовый файл.
    ///     Гарантирует, что все данные, записанные в поток через `write()`, физически сохранены в файле.
    /// \~english @throws iridium::Exception or derived classes if the flush operation fails.
    /// \~russian @throws iridium::Exception или производные классы, если операция сброса не удалась.
    virtual void flush() = 0;
};


/// \~english @brief Interface for readable file streams.
///     Combines file-specific stream properties from `IFileStream` with generic stream reading capabilities from `IStreamReader`.
///     Concrete implementations of this interface will provide the actual logic for reading data from a specific file source.
/// \~russian @brief Интерфейс для читаемых файловых потоков.
///     Объединяет специфичные для файлов свойства потока из `IFileStream` с общими возможностями чтения потока из `IStreamReader`.
///     Конкретные реализации этого интерфейса будут предоставлять фактическую логику для чтения данных из определенного файлового источника.
class IFileStreamReader: public virtual IFileStream, public virtual IStreamReader {
public:
    /// \~english @brief Macro used to define common interface elements.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    DEFINE_INTERFACE(IFileStreamReader)
    // This interface currently adds no new methods beyond those inherited from IFileStream and IStreamReader.
};


// todo: maybe (This section for a combined read/write file stream port is commented out in the source)
/// \~english @brief Potential interface for a duplex file stream port, combining reading, writing, and seeking capabilities.
/// \~russian @brief Потенциальный интерфейс для дуплексного файлового потока, объединяющий возможности чтения, записи и поиска.
//class IFileStreamPort: public IFileStreamReader, public IFileStreamWriter {
//public:
//    virtual void seek(...) = 0;
//}


} // fs
} // io
} // iridium


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
