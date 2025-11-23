// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "iridium/platform.h"
#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "buffer.h"
#include "uri.h"

#include <list>


namespace iridium::io {


static size_t const DEFAULT_BUFFER_SIZE = 4096;


/// \~english @brief Base interface for all stream types.
/// \~english @details Provides fundamental stream properties like underlying system handles and the associated URI.
/// \~russian @brief Базовый интерфейс для всех типов потоков.
/// \~russian @details Предоставляет основные свойства потока, такие как системные дескрипторы и связанный с ним URI.
class IStream: public virtual pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStream)

    /// \~english @brief Get the list of underlying system handles for the stream.
    /// \~english @return A list of uintptr_t representing the native handles (e.g., file descriptors, sockets).
    /// \~russian @brief Получить список системных дескрипторов для потока.
    /// \~russian @return Список uintptr_t, представляющих нативные дескрипторы (например, файловые дескрипторы, сокеты).
    virtual std::list<uintptr_t>    getHandles()    const = 0;

    /// \~english @brief Get the URI associated with the stream.
    /// \~english @return A shared pointer to the URI object.
    /// \~russian @brief Получить URI, связанный с потоком.
    /// \~russian @return Умный указатель на объект URI.
    virtual URI::TSharedPtr         getURI()        const = 0;
};

/// \~english @brief Interface for a readable stream.
/// \~russian @brief Интерфейс для потока, доступного для чтения.
class IStreamReader: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamReader)

    /// \~english @brief Reads data from the stream.
    /// \~english @param size The maximum number of bytes to read. If 0, returns any previously accumulated buffer.
    /// \~english @return A shared pointer to a Buffer.
    /// \~english - a valid Buffer with data if read was successful.
    /// \~english - an empty Buffer if no data is currently available.
    /// \~english - `nullptr` on EOF or if the stream was closed unexpectedly.
    /// \~russian @brief Читает данные из потока.
    /// \~russian @param size Максимальное количество байт для чтения. Если 0, возвращает ранее накопленный буфер.
    /// \~russian @return Умный указатель на буфер.
    /// \~russian - корректный буфер с данными в случае успешного чтения.
    /// \~russian - пустой буфер, если данные в данный момент отсутствуют.
    /// \~russian - `nullptr` при достижении конца файла (EOF) или при неожиданном закрытии потока.
    virtual Buffer::TSharedPtr read(size_t const &size = DEFAULT_BUFFER_SIZE) = 0;
};

/// \~english @brief Interface for a writable stream.
/// \~russian @brief Интерфейс для потока, доступного для записи.
class IStreamWriter: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)

    /// \~english @brief Writes data to the stream.
    /// \~english @param buffer The Buffer containing the data to write.
    /// \~english @return The number of bytes written. Returns 0 if the stream is not ready for writing.
    /// \~russian @brief Записывает данные в поток.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~russian @return Количество записанных байт. Возвращает 0, если поток не готов для записи.
    virtual size_t write(Buffer::TSharedPtr const &buffer) = 0;
};


/// \~english @brief Interface for a stream that supports both reading and writing.
/// \~english @details This interface combines IStreamReader and IStreamWriter, representing a bidirectional communication channel.
/// \~russian @brief Интерфейс для потока, поддерживающего как чтение, так и запись.
/// \~russian @details Этот интерфейс объединяет IStreamReader и IStreamWriter, представляя двунаправленный канал связи.
class IStreamPort:
    public virtual IStreamReader,
    public virtual IStreamWriter
{
public:
    DEFINE_INTERFACE(IStreamPort)
};

/// \~english @brief Interface for a stream acceptor (e.g., a listening socket).
/// \~russian @brief Интерфейс для принимающего потока (например, слушающего сокета).
class IStreamAcceptor: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamAcceptor)

    /// \~english @brief Accepts incoming connections.
    /// \~english @return A list of new IStreamPort shared pointers representing the new connections.
    /// \~english The list may be empty if no new connections are pending.
    /// \~russian @brief Принимает входящие соединения.
    /// \~russian @return Список умных указателей на новые IStreamPort, представляющие новые соединения.
    /// \~russian Список может быть пустым, если нет ожидающих соединений.
    virtual std::list<IStreamPort::TSharedPtr> accept() = 0;
};


} // namespace iridium::io


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
