// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
#define HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB


#include "iridium/io/stream.h"


namespace iridium {
namespace io {
namespace implementation {


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4250)
#endif // _MSC_VER


/// \~english @brief Base class for stream implementations that operate on an underlying memory buffer.
///     Implements common `IStream` methods that are not dependent on read/write direction.
/// \~russian @brief Базовый класс для реализаций потоков, работающих с нижележащим буфером памяти.
///     Реализует общие методы `IStream`, которые не зависят от направления чтения/записи.
class CStreamBuffer: public virtual IStream {
protected:
    /// \~english @brief Constructs a `CStreamBuffer` with a given backing buffer.
    /// \~russian @brief Конструирует `CStreamBuffer` с заданным буфером.
    /// \~english @param buffer The shared buffer that this stream will operate on.
    /// \~russian @param buffer Разделяемый буфер, с которым будет работать этот поток.
    explicit CStreamBuffer(Buffer::TSharedPtr const &buffer);
    
    /// \~english @brief Initializes the stream. (Currently a no-op for buffer-based streams).
    /// \~russian @brief Инициализирует поток. (В настоящее время не выполняет никаких действий для потоков на основе буфера).
    void initialize() override;
    /// \~english @brief Finalizes the stream. (Currently a no-op for buffer-based streams).
    /// \~russian @brief Завершает работу потока. (В настоящее время не выполняет никаких действий для потоков на основе буфера).
    void finalize()   override;
    
    /// \~english @brief Gets underlying OS handles. Not applicable for buffer streams.
    /// \~russian @brief Получает базовые дескрипторы ОС. Неприменимо для буферных потоков.
    /// \~english @return An empty list, as buffer streams do not have OS handles.
    /// \~russian @return Пустой список, так как буферные потоки не имеют дескрипторов ОС.
    std::list<uintptr_t>    getHandles()    const override;
    /// \~english @brief Gets the URI for this stream. Not typically applicable for memory buffer streams.
    /// \~russian @brief Получает URI для этого потока. Обычно неприменимо для потоков буфера памяти.
    /// \~english @return `nullptr`, as generic buffer streams don't have a URI.
    /// \~russian @return `nullptr`, так как у общих буферных потоков нет URI.
    URI::TSharedPtr         getURI()        const override;

    /// \~english @brief The underlying shared memory buffer used by this stream.
    /// \~russian @brief Нижележащий разделяемый буфер памяти, используемый этим потоком.
    Buffer::TSharedPtr m_buffer;
};


/// \~english @brief Implements a readable stream (`IStreamReader`) that reads from a `Buffer`.
///     The reading is non-destructive to the original buffer; it maintains an internal position.
/// \~russian @brief Реализует читаемый поток (`IStreamReader`), который читает из `Buffer`.
///     Чтение не разрушает исходный буфер; он поддерживает внутреннюю позицию.
class CStreamReaderBuffer: 
    public CStreamBuffer,
    public IStreamReader
{
public:
    DEFINE_IMPLEMENTATION(CStreamReaderBuffer)
    /// \~english @brief Constructs a `CStreamReaderBuffer` that reads from the provided buffer.
    /// \~russian @brief Конструирует `CStreamReaderBuffer`, который читает из предоставленного буфера.
    /// \~english @param buffer The buffer to read data from.
    /// \~russian @param buffer Буфер для чтения данных.
    explicit CStreamReaderBuffer(Buffer::TSharedPtr const &buffer);
    
    /// \~english @brief Reads up to `size` bytes from the current position in the buffer.
    ///     Advances the internal read position by the number of bytes read.
    /// \~russian @brief Читает до `size` байт из текущей позиции в буфере.
    ///     Сдвигает внутреннюю позицию чтения на количество прочитанных байт.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A new `Buffer` containing the data read, or `nullptr` if EOF is reached (no more data to read).
    /// \~russian @return Новый `Buffer`, содержащий прочитанные данные, или `nullptr`, если достигнут конец файла (больше нет данных для чтения).
    Buffer::TSharedPtr read(size_t const &size) override;

private:
    /// \~english @brief The current read position within the `m_buffer`.
    /// \~russian @brief Текущая позиция чтения в `m_buffer`.
    size_t m_pos;
};


// todo: interface with getBuffer
/// \~english @brief Implements a writable stream (`IStreamWriter`) that writes to a `Buffer`.
///     The writing appends data to the end of the provided buffer.
/// \~russian @brief Реализует записываемый поток (`IStreamWriter`), который пишет в `Buffer`.
///     Запись добавляет данные в конец предоставленного буфера.
class CStreamWriterBuffer: 
    public CStreamBuffer,
    public IStreamWriter
{
public:
    DEFINE_IMPLEMENTATION(CStreamWriterBuffer)
    /// \~english @brief Constructs a `CStreamWriterBuffer` that writes to the provided buffer.
    /// \~russian @brief Конструирует `CStreamWriterBuffer`, который пишет в предоставленный буфер.
    /// \~english @param buffer The buffer to write data to.
    /// \~russian @param buffer Буфер для записи данных.
    explicit CStreamWriterBuffer(Buffer::TSharedPtr const &buffer);

    /// \~english @brief Writes the content of the given `buffer` into this stream's underlying buffer.
    ///     The data is appended to the end of the stream's buffer.
    /// \~russian @brief Записывает содержимое данного `buffer` в нижележащий буфер этого потока.
    ///     Данные добавляются в конец буфера потока.
    /// \~english @param buffer The buffer containing data to write.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~english @return The number of bytes written (equal to the size of the input buffer).
    /// \~russian @return Количество записанных байт (равное размеру входного буфера).
    /// \~english @throws std::bad_alloc if resizing the internal buffer fails.
    /// \~russian @throws std::bad_alloc, если не удалось изменить размер внутреннего буфера.
    size_t write(Buffer::TSharedPtr const &buffer) override;
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
