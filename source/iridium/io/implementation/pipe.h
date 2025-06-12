// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
#define HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D


#include "iridium/io/pipe.h"
#include "iridium/io/stream.h"


namespace iridium {
namespace io {
namespace implementation {


/// \~english @brief Concrete implementation of the `IPipe` interface.
///     A `CPipe` acts as a conduit, typically between a logical stream (represented by its reader/writer)
///     and a physical or multiplexed stream (represented by the reader/writer set via the `set` method).
///     It can buffer data and handles the transmission of data based on events.
/// \~russian @brief Конкретная реализация интерфейса `IPipe`.
///     `CPipe` действует как канал, обычно между логическим потоком (представленным его читателем/писателем)
///     и физическим или мультиплексированным потоком (представленным читателем/писателем, установленным через метод `set`).
///     Он может буферизировать данные и обрабатывает передачу данных на основе событий.
class CPipe: public IPipe {
public:
    DEFINE_CREATE(CPipe)

    /// \~english @brief Default size for individual buffers used within the pipe, in bytes.
    /// \~russian @brief Размер по умолчанию для отдельных буферов, используемых в канале, в байтах.
    static size_t const DEFAULT_BUFFER_SIZE;
    /// \~english @brief Default number of buffers to pre-allocate or manage within the pipe.
    /// \~russian @brief Количество буферов по умолчанию для предварительного выделения или управления в канале.
    static size_t const DEFAULT_BUFFER_COUNT;

    /// \~english @brief Constructs a `CPipe` instance with default buffer settings.
    ///     Initializes internal buffer management parameters.
    /// \~russian @brief Конструирует экземпляр `CPipe` с настройками буфера по умолчанию.
    ///     Инициализирует внутренние параметры управления буфером.
    CPipe();
    /// \~english @brief Virtual destructor.
    /// \~russian @brief Виртуальный деструктор.
    virtual ~CPipe();

    /// \~english @brief Sets the underlying reader and writer for this pipe.
    ///     These are typically the endpoints of the actual communication channel (e.g., a socket stream or a multiplexed sub-stream)
    ///     to which data from the pipe's logical reader should be written, and from which data for the pipe's logical writer should be read.
    /// \~russian @brief Устанавливает базовые читателя и писателя для этого канала.
    ///     Обычно это конечные точки фактического канала связи (например, сокетного потока или мультиплексированного подпотока),
    ///     в которые должны записываться данные от логического читателя канала и из которых должны читаться данные для логического писателя канала.
    /// \~english @param reader The stream reader that data will be read from (source for the pipe's writer).
    /// \~russian @param reader Читатель потока, из которого будут читаться данные (источник для писателя канала).
    /// \~english @param writer The stream writer that data will be written to (destination for the pipe's reader).
    /// \~russian @param writer Писатель потока, в который будут записываться данные (получатель для читателя канала).
    void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) override;

    /// \~english @brief Gets the logical stream reader associated with this pipe.
    ///     This reader is used by the application to read data that has been received and buffered by the pipe.
    /// \~russian @brief Получает логического читателя потока, связанного с этим каналом.
    ///     Этот читатель используется приложением для чтения данных, которые были получены и буферизованы каналом.
    /// \~english @return A shared pointer to the pipe's `IStreamReader`.
    /// \~russian @return Умный указатель на `IStreamReader` канала.
    IStreamReader::TSharedPtr getReader() const override;

    /// \~english @brief Gets the logical stream writer associated with this pipe.
    ///     This writer is used by the application to write data that should be transmitted through the pipe.
    /// \~russian @brief Получает логического писателя потока, связанного с этим каналом.
    ///     Этот писатель используется приложением для записи данных, которые должны быть переданы через канал.
    /// \~english @return A shared pointer to the pipe's `IStreamWriter`.
    /// \~russian @return Умный указатель на `IStreamWriter` канала.
    IStreamWriter::TSharedPtr getWriter() const override;

    /// \~english @brief Transmits data based on the provided event.
    ///     If the event indicates data is available on the pipe's logical reader (`m_reader`), this method reads from it,
    ///     buffers the data, and attempts to write it to the underlying stream writer (`m_writer`).
    ///     It manages internal buffers for this process.
    /// \~russian @brief Передает данные на основе предоставленного события.
    ///     Если событие указывает, что данные доступны в логическом читателе канала (`m_reader`), этот метод читает из него,
    ///     буферизирует данные и пытается записать их в базовый писатель потока (`m_writer`).
    ///     Он управляет внутренними буферами для этого процесса.
    /// \~english @param event The event triggering the transmission (e.g., `Event::READ_STREAM` on the pipe's reader).
    /// \~russian @param event Событие, инициирующее передачу (например, `Event::READ_STREAM` на читателе канала).
    /// \~english @return `true` if data was successfully read and queued for writing (or written), `false` on error or if no action was taken.
    /// \~russian @return `true`, если данные были успешно прочитаны и поставлены в очередь на запись (или записаны), `false` в случае ошибки или если никаких действий не было предпринято.
    bool transmit(Event::TConstSharedPtr const &event) override;

private:
    /// \~english @brief The logical stream reader for this pipe, used by the application to consume data.
    /// \~russian @brief Логический читатель потока для этого канала, используемый приложением для потребления данных.
    IStreamReader::TSharedPtr       m_reader;
    /// \~english @brief The logical stream writer for this pipe, used by the application to produce data.
    /// \~russian @brief Логический писатель потока для этого канала, используемый приложением для производства данных.
    IStreamWriter::TSharedPtr       m_writer;
    /// \~english @brief The size of each individual buffer in `m_buffers`.
    /// \~russian @brief Размер каждого отдельного буфера в `m_buffers`.
    size_t const                    m_buffer_size;
    /// \~english @brief The maximum number of buffers to maintain in `m_buffers`.
    /// \~russian @brief Максимальное количество буферов для хранения в `m_buffers`.
    size_t const                    m_buffer_count;
    /// \~english @brief A list of buffers used for staging data read from `m_reader` before it's written to `m_writer`.
    /// \~russian @brief Список буферов, используемых для промежуточного хранения данных, прочитанных из `m_reader`, перед их записью в `m_writer`.
    std::list<Buffer::TSharedPtr>   m_buffers;
};
    

} // implementation
} // io
} // iridium


#endif // HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
