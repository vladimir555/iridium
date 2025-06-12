#ifndef HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E
#define HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E


#include "iridium/io/event.h"
#include "iridium/io/protocol.h"
#include "iridium/io/context.h"
#include "iridium/io/pipe.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <chrono>


namespace iridium {
namespace io {
namespace implementation {


/// \~english @brief Concrete implementation of `IContext` and `IPipeManager`.
///     This class manages the state and events for a communication context, typically associated with a single main stream (e.g., a client connection).
///     It handles event queuing, protocol-based event processing, and management of named pipes for multiplexing data over the main stream.
///     It uses a mutex for synchronization of its operations.
/// \~russian @brief Конкретная реализация `IContext` и `IPipeManager`.
///     Этот класс управляет состоянием и событиями для контекста обмена данными, обычно связанного с одним основным потоком (например, клиентским подключением).
///     Он обрабатывает постановку событий в очередь, обработку событий на основе протокола и управление именованными каналами (pipes) для мультиплексирования данных через основной поток.
///     Использует мьютекс для синхронизации своих операций.
class CContext:
    public IContext,
    public IPipeManager,
    public threading::Synchronized<std::mutex>,
    public std::enable_shared_from_this<CContext>
{
public:
    DEFINE_CREATE(CContext)
    
    /// \~english @brief Constructs a `CContext` instance.
    /// \~russian @brief Конструирует экземпляр `CContext`.
    /// \~english @param stream The primary stream associated with this context. Events are typically read from and written to this stream via the protocol.
    /// \~russian @param stream Основной поток, связанный с этим контекстом. События обычно читаются из этого потока и записываются в него через протокол.
    /// \~english @param protocol The protocol instance used to encode and decode events for the stream.
    /// \~russian @param protocol Экземпляр протокола, используемый для кодирования и декодирования событий для потока.
    CContext(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol);
    
    /// \~english @brief Pushes an event onto the internal event queue of this context.
    /// \~russian @brief Помещает событие во внутреннюю очередь событий этого контекста.
    /// \~english @param event The event to be added to the queue.
    /// \~russian @param event Событие для добавления в очередь.
    void pushEvent(Event::TSharedPtr const &event) override;

    /// \~english @brief Pops all currently pending events from the internal event queue.
    /// \~russian @brief Извлекает все ожидающие события из внутренней очереди событий.
    /// \~english @return A list of events that were pending in the queue.
    /// \~russian @return Список событий, которые находились в очереди.
    std::list<Event::TSharedPtr>
         popEvents() override;

    /// \~english @brief Checks for outdated streams (pipes) associated with this context and generates close events for them.
    ///     A stream is considered outdated based on a timeout mechanism.
    /// \~russian @brief Проверяет наличие устаревших потоков (каналов), связанных с этим контекстом, и генерирует для них события закрытия.
    ///     Поток считается устаревшим на основе механизма тайм-аута.
    /// \~english @return A list of `Event::CLOSE_STREAM` events for any streams deemed outdated.
    /// \~russian @return Список событий `Event::CLOSE_STREAM` для всех потоков, признанных устаревшими.
    std::list<Event::TSharedPtr>
         checkOutdatedStreams() override;
    
    /// \~english @brief Updates the context based on an incoming event, typically by processing it with the protocol.
    ///     This may involve reading data from the event's stream and generating new events.
    /// \~russian @brief Обновляет контекст на основе входящего события, обычно обрабатывая его с помощью протокола.
    ///     Это может включать чтение данных из потока события и генерацию новых событий.
    /// \~english @param event The event to process (e.g., `Event::READ_STREAM`).
    /// \~russian @param event Событие для обработки (например, `Event::READ_STREAM`).
    /// \~english @return `true` if the update was successful, `false` otherwise (e.g., protocol error, stream error).
    /// \~russian @return `true`, если обновление прошло успешно, иначе `false` (например, ошибка протокола, ошибка потока).
    bool update    (Event::TSharedPtr const &event) override;

    /// \~english @brief Transmits an event, typically by encoding it with the protocol and writing it to the event's stream.
    /// \~russian @brief Передает событие, обычно кодируя его с помощью протокола и записывая в поток события.
    /// \~english @param event The event to transmit (e.g., `Event::DATA`).
    /// \~russian @param event Событие для передачи (например, `Event::DATA`).
    /// \~english @return `true` if the transmission was successful, `false` otherwise (e.g., protocol error, stream error).
    /// \~russian @return `true`, если передача прошла успешно, иначе `false` (например, ошибка протокола, ошибка потока).
    bool transmit  (Event::TSharedPtr const &event) override;
    
    /// \~english @brief Creates a new named pipe within this context.
    /// \~russian @brief Создает новый именованный канал (pipe) в этом контексте.
    /// \~english @param name The unique name for the new pipe.
    /// \~russian @param name Уникальное имя для нового канала.
    /// \~english @throws iridium::Exception if a pipe with the same name already exists.
    /// \~russian @throws iridium::Exception, если канал с таким именем уже существует.
    void createPipe(std::string const &name) override;

    /// \~english @brief Removes a named pipe from this context.
    ///     Also generates a `Event::CLOSE_STREAM` for the associated pipe stream.
    /// \~russian @brief Удаляет именованный канал (pipe) из этого контекста.
    ///     Также генерирует `Event::CLOSE_STREAM` для связанного потока канала.
    /// \~english @param name The name of the pipe to remove.
    /// \~russian @param name Имя удаляемого канала.
    void removePipe(std::string const &name) override;

    /// \~english @brief Updates the reader endpoint of a named pipe.
    /// \~russian @brief Обновляет конечную точку чтения именованного канала.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader The new stream reader for the pipe.
    /// \~russian @param reader Новый читатель потока для канала.
    /// \~english @throws iridium::Exception if the pipe with the given name does not exist.
    /// \~russian @throws iridium::Exception, если канал с указанным именем не существует.
    void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) override;

    /// \~english @brief Updates the writer endpoint of a named pipe.
    /// \~russian @brief Обновляет конечную точку записи именованного канала.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param writer The new stream writer for the pipe.
    /// \~russian @param writer Новый писатель потока для канала.
    /// \~english @throws iridium::Exception if the pipe with the given name does not exist.
    /// \~russian @throws iridium::Exception, если канал с указанным именем не существует.
    void updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) override;

    /// \~english @brief Updates both the reader and writer endpoints of a named pipe.
    /// \~russian @brief Обновляет как конечную точку чтения, так и конечную точку записи именованного канала.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader The new stream reader for the pipe.
    /// \~russian @param reader Новый читатель потока для канала.
    /// \~english @param writer The new stream writer for the pipe.
    /// \~russian @param writer Новый писатель потока для канала.
    /// \~english @throws iridium::Exception if the pipe with the given name does not exist.
    /// \~russian @throws iridium::Exception, если канал с указанным именем не существует.
    void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader,
                                             IStreamWriter::TSharedPtr const &writer) override;

    /// \~english @brief Removes the context, effectively closing all associated pipes and the main stream.
    ///     Generates `Event::CLOSE_STREAM` events for all managed pipe streams and the main context stream.
    /// \~russian @brief Удаляет контекст, фактически закрывая все связанные каналы и основной поток.
    ///     Генерирует события `Event::CLOSE_STREAM` для всех управляемых потоков каналов и основного потока контекста.
    void remove() override;

private:
    /// \~english @brief Internal helper to remove a pipe and its associated stream, generating a close event.
    /// \~russian @brief Внутренний вспомогательный метод для удаления канала и связанного с ним потока, генерируя событие закрытия.
    /// \~english @param pipe The pipe to remove.
    /// \~russian @param pipe Канал для удаления.
    void removePipe(IPipe::TSharedPtr const &pipe);

    /// \~english @brief Internal helper to remove a stream (main or pipe) and optionally generate a close event.
    /// \~russian @brief Внутренний вспомогательный метод для удаления потока (основного или канала) и опциональной генерации события закрытия.
    /// \~english @param stream The stream to remove.
    /// \~russian @param stream Поток для удаления.
    /// \~english @param is_send_close_event If true, an `Event::CLOSE_STREAM` is generated for this stream.
    /// \~russian @param is_send_close_event Если true, генерируется `Event::CLOSE_STREAM` для этого потока.
    void removeStream(IStream::TSharedPtr const &stream, bool const &is_send_close_event = true);
    
    /// \~english @brief Asynchronous queue for storing incoming events to be processed by this context.
    /// \~russian @brief Асинхронная очередь для хранения входящих событий, которые должны быть обработаны этим контекстом.
    threading::IAsyncQueue<Event::TSharedPtr>::TSharedPtr
        m_events;

    /// \~english @brief The protocol instance used for encoding/decoding events related to this context's streams.
    /// \~russian @brief Экземпляр протокола, используемый для кодирования/декодирования событий, связанных с потоками этого контекста.
    IProtocol::TSharedPtr
        m_protocol;

    /// \~english @brief Map storing named pipes, associating a pipe name (string) with its `IPipe` instance.
    /// \~russian @brief Карта, хранящая именованные каналы, связывая имя канала (строку) с его экземпляром `IPipe`.
    std::unordered_map<std::string, IPipe::TSharedPtr>
        m_map_name_pipe;

    /// \~english @brief Map associating a stream (typically a pipe's internal stream) with its managing `IPipe` instance.
    ///     This allows finding the pipe when an event occurs on one of its streams.
    /// \~russian @brief Карта, связывающая поток (обычно внутренний поток канала) с его управляющим экземпляром `IPipe`.
    ///     Это позволяет находить канал при возникновении события в одном из его потоков.
    std::unordered_map<IStream::TSharedPtr, IPipe::TSharedPtr>
        m_map_stream_pipe;

    /// \~english @brief Map storing the last activity timestamp for each stream associated with this context.
    ///     Used by `checkOutdatedStreams` to detect inactivity.
    /// \~russian @brief Карта, хранящая временную метку последней активности для каждого потока, связанного с этим контекстом.
    ///     Используется `checkOutdatedStreams` для обнаружения неактивности.
    std::unordered_map<IStream::TSharedPtr, std::chrono::system_clock::time_point>
        m_map_stream_timestamp;
};


} // implementation
} // io
} // iridium


#endif // HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E
