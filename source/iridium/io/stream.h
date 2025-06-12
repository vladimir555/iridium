// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines core interfaces for I/O stream operations, including reading, writing, and stream properties.
/// \~russian @brief Определяет основные интерфейсы для операций потокового ввода-вывода, включая чтение, запись и свойства потока.
#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "iridium/platform.h"
#include "iridium/smart_ptr.h"           // For DEFINE_INTERFACE, TSharedPtr
#include "iridium/pattern/initializable.h" // For IInitializable

#include "buffer.h"   // For Buffer::TSharedPtr
#include "uri.h"      // For URI::TSharedPtr

#include <list>       // For std::list
#include <cstddef>    // For size_t
#include <cstdint>    // For uintptr_t
//#include <functional> // Commented out in source


namespace iridium {
namespace io {


/// \~english @brief Default buffer size used for read operations if no specific size is requested.
/// \~russian @brief Размер буфера по умолчанию, используемый для операций чтения, если не указан конкретный размер.
static size_t const DEFAULT_BUFFER_SIZE = 65535;


/// \~english @brief Base interface for I/O streams.
///     Provides common functionalities for all stream types, such as accessing underlying OS handles
///     and retrieving an associated URI. It also inherits from `IInitializable`, implying streams
///     may require an initialization step. This interface uses virtual inheritance to allow
///     derived interfaces like `IStreamPort` to inherit from multiple stream-related interfaces (`IStreamReader`, `IStreamWriter`) without ambiguity.
/// \~russian @brief Базовый интерфейс для потоков ввода-вывода.
///     Предоставляет общие функциональные возможности для всех типов потоков, такие как доступ
///     к базовым системным дескрипторам и получение связанного URI. Он также наследуется от `IInitializable`,
///     подразумевая, что потоки могут требовать шага инициализации. Этот интерфейс использует виртуальное наследование,
///     чтобы позволить производным интерфейсам, таким как `IStreamPort`, наследовать от нескольких интерфейсов,
///     связанных с потоками (`IStreamReader`, `IStreamWriter`), без неоднозначности.
class IStream: public virtual pattern::IInitializable {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IStream)

    /// \~english @brief Retrieves a list of underlying operating system handles associated with this stream.
    ///     For example, for a socket stream, this might return the socket file descriptor.
    ///     For other stream types, it could be file handles or other OS-specific identifiers.
    /// \~russian @brief Получает список базовых дескрипторов операционной системы, связанных с этим потоком.
    ///     Например, для сокетного потока это может вернуть файловый дескриптор сокета.
    ///     Для других типов потоков это могут быть файловые дескрипторы или другие специфичные для ОС идентификаторы.
    /// \~english @return A list of `uintptr_t` representing the OS handles. The list may be empty if no handles are applicable.
    /// \~russian @return Список `uintptr_t`, представляющих дескрипторы ОС. Список может быть пустым, если дескрипторы отсутствуют.
    virtual std::list<uintptr_t>    getHandles()    const = 0;

    /// \~english @brief Retrieves the URI associated with this stream, if any.
    ///     This can be used to identify the source or destination of the stream (e.g., a file path, network address).
    /// \~russian @brief Получает URI, связанный с этим потоком, если таковой имеется.
    ///     Может использоваться для идентификации источника или назначения потока (например, путь к файлу, сетевой адрес).
    /// \~english @return A shared pointer to a `URI` object, or `nullptr` if no URI is associated with the stream.
    /// \~russian @return Умный указатель на объект `URI` или `nullptr`, если с потоком не связан URI.
    virtual URI::TSharedPtr         getURI()        const = 0;
};


/// \~english @brief Interface for readable I/O streams.
///     Defines the contract for streams from which data can be read.
///     It inherits virtually from `IStream` to allow combination with `IStreamWriter` in `IStreamPort`.
/// \~russian @brief Интерфейс для читаемых потоков ввода-вывода.
///     Определяет контракт для потоков, из которых можно читать данные.
///     Наследуется виртуально от `IStream`, чтобы обеспечить возможность комбинации с `IStreamWriter` в `IStreamPort`.
class IStreamReader: public virtual IStream {
public:
    /// \~english @brief Macro used to define common interface elements.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    DEFINE_INTERFACE(IStreamReader)

    /// \~english @brief Reads data from the stream up to a specified size.
    ///     Behavior based on stream state:
    ///     - Returns `nullptr` if End-Of-File (EOF) is reached or the stream is unexpectedly closed.
    ///     - Returns an empty (but not null) `Buffer` if no data is currently available (e.g., in non-blocking mode).
    ///     - If `size` is 0, it may return a previously accumulated internal buffer, if such a mechanism exists in the implementation.
    /// \~russian @brief Читает данные из потока размером до указанного.
    ///     Поведение зависит от состояния потока:
    ///     - Возвращает `nullptr` при достижении конца файла (EOF) или при неожиданном закрытии потока.
    ///     - Возвращает пустой (но не null) `Buffer`, если данные в данный момент недоступны (например, в неблокирующем режиме).
    ///     - Если `size` равен 0, может вернуть ранее накопленный внутренний буфер, если такой механизм существует в реализации.
    /// \~english @param size The maximum number of bytes to read. Defaults to `DEFAULT_BUFFER_SIZE`.
    ///     A size of 0 may have special meaning (see description).
    /// \~russian @param size Максимальное количество байт для чтения. По умолчанию `DEFAULT_BUFFER_SIZE`.
    ///     Нулевой размер может иметь специальное значение (см. описание).
    /// \~english @return A shared pointer to a `Buffer` containing the data read.
    ///     See remarks for interpretation of `nullptr` or an empty Buffer.
    /// \~russian @return Умный указатель на `Buffer`, содержащий прочитанные данные.
    ///     См. примечания для интерпретации `nullptr` или пустого Buffer.
    virtual Buffer::TSharedPtr read(size_t const &size = DEFAULT_BUFFER_SIZE) = 0;
};


/// \~english @brief Interface for writable I/O streams.
///     Defines the contract for streams to which data can be written.
///     It inherits virtually from `IStream` to allow combination with `IStreamReader` in `IStreamPort`.
/// \~russian @brief Интерфейс для записываемых потоков ввода-вывода.
///     Определяет контракт для потоков, в которые можно записывать данные.
///     Наследуется виртуально от `IStream`, чтобы обеспечить возможность комбинации с `IStreamReader` в `IStreamPort`.
class IStreamWriter: public virtual IStream {
public:
    /// \~english @brief Macro used to define common interface elements.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    DEFINE_INTERFACE(IStreamWriter)

    /// \~english @brief Writes data from a buffer to the stream.
    ///     Attempts to write the contents of the provided buffer to the stream.
    /// \~russian @brief Записывает данные из буфера в поток.
    ///     Пытается записать содержимое предоставленного буфера в поток.
    /// \~english @param buffer A shared pointer to a `Buffer` containing the data to be written.
    /// \~russian @param buffer Умный указатель на `Buffer`, содержащий данные для записи.
    /// \~english @return The number of bytes actually written. This may be less than the buffer size
    ///     if the stream is not ready to accept all data (e.g., in non-blocking mode).
    ///     A return value of 0 can indicate that the stream (e.g., socket) is not ready for writing.
    /// \~russian @return Количество фактически записанных байт. Это значение может быть меньше размера буфера,
    ///     если поток не готов принять все данные (например, в неблокирующем режиме).
    ///     Возвращаемое значение 0 может указывать на то, что поток (например, сокет) не готов к записи.
    virtual size_t write(Buffer::TSharedPtr const &buffer) = 0;
};


/// \~english @brief Interface for a duplex I/O stream port, combining reading and writing capabilities.
///     This interface inherits from both `IStreamReader` and `IStreamWriter`, representing
///     a bidirectional communication channel. It also inherits virtually from the common `IStream` base.
/// \~russian @brief Интерфейс для дуплексного порта потока ввода-вывода, объединяющий возможности чтения и записи.
///     Этот интерфейс наследуется как от `IStreamReader`, так и от `IStreamWriter`, представляя
///     двунаправленный канал связи. Он также виртуально наследуется от общего базового класса `IStream`.
class IStreamPort:
    public virtual IStreamReader,
    public virtual IStreamWriter
{
public:
    /// \~english @brief Macro used to define common interface elements.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    DEFINE_INTERFACE(IStreamPort)
};


// Commented out operator overloads in the source:
//bool operator < (IStream::TSharedPtr        const &l, IStream::TSharedPtr       const &r);
// ... (and many other combinations)


} // io
} // iridium


// Commented out std::hash specialization in the source:
//template<>
//struct std::hash<iridium::io::IStream::TSharedPtr> {
//    std::size_t operator()(iridium::io::IStream::TSharedPtr const &stream) const;
//};


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
