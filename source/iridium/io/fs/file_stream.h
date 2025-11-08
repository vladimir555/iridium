// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "iridium/io/stream.h"
#include "iridium/io/fs/file_status.h"


namespace iridium {
namespace io {
namespace fs {


/// \~english @brief Base interface for file streams, extending IStream with file-specific operations.
/// \~russian @brief Базовый интерфейс для файловых потоков, расширяющий IStream специфичными для файлов операциями.
class IFileStream : public virtual IStream
{
public:
    DEFINE_INTERFACE(IFileStream)
    /// \~english @brief Gets the status of the file.
    /// \~russian @brief Возвращает статус файла.
    /// \~english @return A TFileStatus object containing information like last modification time and size.
    /// \~russian @return Объект TFileStatus, содержащий информацию, такую как время последнего изменения и размер.
    virtual TFileStatus getStatus() const = 0;
};

/// \~english @brief Interface for a writable file stream.
/// \~russian @brief Интерфейс для файлового потока, доступного для записи.
class IFileStreamWriter : public virtual IFileStream, public virtual IStreamWriter
{
public:
    DEFINE_INTERFACE(IFileStreamWriter)
    /// \~english @brief Flushes any buffered data to the underlying file.
    /// \~russian @brief Сбрасывает все буферизованные данные в нижележащий файл.
    virtual void flush() = 0;
};

/// \~english @brief Interface for a readable file stream.
/// \~russian @brief Интерфейс для файлового потока, доступного для чтения.
class IFileStreamReader : public virtual IFileStream, public virtual IStreamReader
{
public:
    DEFINE_INTERFACE(IFileStreamReader)
};


// todo: maybe
//class IFileStreamPort: public IFileStreamReader, public IFileStreamWriter {
//public:
//    virtual void seek(...) = 0;
//}


} // fs
} // io
} // iridium


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
