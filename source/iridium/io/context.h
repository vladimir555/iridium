// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines interfaces for I/O context and pipe management.
/// \~russian @file
/// @brief Определяет интерфейсы для управления контекстом ввода-вывода и каналами.

#ifndef HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
#define HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6


#include "event.h" // For Event::TSharedPtr


namespace iridium::io {


/// \~english @brief Interface for an I/O context, responsible for managing and dispatching I/O events.
///     An I/O context typically handles asynchronous operations, event notifications,
///     and management of I/O resources like streams or sockets.
/// \~russian @brief Интерфейс для контекста ввода-вывода, отвечающего за управление и диспетчеризацию событий ввода-вывода.
///     Контекст ввода-вывода обычно обрабатывает асинхронные операции, уведомления о событиях
///     и управление ресурсами ввода-вывода, такими как потоки или сокеты.
class IContext {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IContext)

    /// \~english @brief Pushes an event into the context's event queue.
    /// \~russian @brief Помещает событие в очередь событий контекста.
    /// \~english @param event A shared pointer to the event to be pushed.
    /// \~russian @param event Умный указатель на событие, которое нужно поместить в очередь.
    virtual void pushEvent(Event::TSharedPtr const &event) = 0;

    /// \~english @brief Pops (retrieves and removes) events from the context's event queue.
    ///     This method is typically called by an event loop to get pending events for dispatching.
    /// \~russian @brief Извлекает (получает и удаляет) события из очереди событий контекста.
    ///     Этот метод обычно вызывается циклом событий для получения ожидающих событий для диспетчеризации.
    /// \~english @return A list of shared pointers to events that were popped from the queue.
    ///     The list may be empty if no events are pending.
    /// \~russian @return Список умных указателей на события, извлеченные из очереди.
    ///     Список может быть пустым, если нет ожидающих событий.
    virtual std::list<Event::TSharedPtr>
                 popEvents() = 0;

    /// \~english @brief Checks for and retrieves events related to outdated or timed-out I/O streams.
    ///     This could be used to manage stream lifecycles, detect idle connections, or handle timeouts.
    /// \~russian @brief Проверяет и извлекает события, связанные с устаревшими или просроченными потоками ввода-вывода.
    ///     Может использоваться для управления жизненным циклом потоков, обнаружения неактивных соединений или обработки тайм-аутов.
    /// \~english @return A list of shared pointers to events representing outdated streams or timeout notifications.
    /// \~russian @return Список умных указателей на события, представляющие устаревшие потоки или уведомления о тайм-ауте.
    virtual std::list<Event::TSharedPtr>
                 checkOutdatedStreams() = 0;

    /// \~english @brief Updates the context's handling of an I/O source based on an event.
    ///     This might involve re-registering a file descriptor with an event multiplexer (e.g., epoll, kqueue)
    ///     with new interest flags (e.g., read, write) based on the event's content or type.
    /// \~russian @brief Обновляет обработку источника ввода-вывода контекстом на основе события.
    ///     Это может включать перерегистрацию файлового дескриптора в мультиплексоре событий (например, epoll, kqueue)
    ///     с новыми флагами интереса (например, чтение, запись) на основе содержимого или типа события.
    /// \~english @param event A shared pointer to the event triggering the update.
    /// \~russian @param event Умный указатель на событие, инициирующее обновление.
    /// \~english @return True if the update was successful, false otherwise.
    /// \~russian @return True, если обновление прошло успешно, иначе false.
    virtual bool update  (Event::TSharedPtr const &event) = 0;

    /// \~english @brief Transmits data or handles I/O operations based on an event.
    ///     This method likely initiates or continues data transfer (read/write) for an I/O source
    ///     associated with the event, or performs other I/O operations as indicated by the event.
    /// \~russian @brief Передает данные или обрабатывает операции ввода-вывода на основе события.
    ///     Этот метод, вероятно, инициирует или продолжает передачу данных (чтение/запись) для источника ввода-вывода,
    ///     связанного с событием, или выполняет другие операции ввода-вывода, указанные событием.
    /// \~english @param event A shared pointer to the event triggering the transmission.
    /// \~russian @param event Умный указатель на событие, инициирующее передачу.
    /// \~english @return True if the transmission was successful or successfully initiated, false otherwise.
    /// \~russian @return True, если передача прошла успешно или была успешно инициирована, иначе false.
    virtual bool transmit(Event::TSharedPtr const &event) = 0;

    /// \~english @brief Removes or cleans up resources associated with this context or its I/O sources.
    ///     The exact scope of removal (e.g., specific stream, all streams, or the context itself
    ///     from a higher-level manager) depends on the concrete implementation.
    ///     Given it takes no arguments, it might signal the context to clean up all its managed resources
    ///     or unregister itself.
    /// \~russian @brief Удаляет или очищает ресурсы, связанные с этим контекстом или его источниками ввода-вывода.
    ///     Точный объем удаления (например, конкретный поток, все потоки или сам контекст
    ///     из менеджера более высокого уровня) зависит от конкретной реализации.
    ///     Учитывая отсутствие аргументов, это может сигнализировать контексту об очистке всех управляемых им ресурсов
    ///     или о снятии с регистрации самого себя.
    virtual void remove  () = 0;
};


/// \~english @brief Interface for managing named pipes.
///     Provides methods to create, remove, and update named pipes by associating them
///     with stream reader and/or writer interfaces. This suggests a mechanism for
///     inter-process or intra-process communication using named pipe abstractions.
/// \~russian @brief Интерфейс для управления именованными каналами.
///     Предоставляет методы для создания, удаления и обновления именованных каналов путем
///     связывания их с интерфейсами чтения и/или записи потоков. Это предполагает механизм
///     межпроцессного или внутрипроцессного взаимодействия с использованием абстракций именованных каналов.
class IPipeManager {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IPipeManager)

    /// \~english @brief Creates a new named pipe.
    /// \~russian @brief Создает новый именованный канал.
    /// \~english @param name The name of the pipe to create.
    /// \~russian @param name Имя создаваемого канала.
    virtual void createPipe(std::string const &name) = 0;

    /// \~english @brief Removes an existing named pipe.
    /// \~russian @brief Удаляет существующий именованный канал.
    /// \~english @param name The name of the pipe to remove.
    /// \~russian @param name Имя удаляемого канала.
    /// \~english @throws iridium::Exception or similar if removal fails (e.g., pipe not found, permissions).
    /// \~russian @throws iridium::Exception или аналогичное, если удаление не удалось (например, канал не найден, проблемы с разрешениями).
    virtual void removePipe(std::string const &name) = 0;

    /// \~english @brief Associates a stream reader with the named pipe.
    ///     This allows data to be read from the pipe via the provided `IStreamReader` interface.
    /// \~russian @brief Связывает читателя потока с именованным каналом.
    ///     Это позволяет читать данные из канала через предоставленный интерфейс `IStreamReader`.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader A shared pointer to an `IStreamReader` implementation.
    /// \~russian @param reader Умный указатель на реализацию `IStreamReader`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) = 0;

    /// \~english @brief Associates a stream writer with the named pipe.
    ///     This allows data to be written to the pipe via the provided `IStreamWriter` interface.
    /// \~russian @brief Связывает писателя потока с именованным каналом.
    ///     Это позволяет записывать данные в канал через предоставленный интерфейс `IStreamWriter`.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param writer A shared pointer to an `IStreamWriter` implementation.
    /// \~russian @param writer Умный указатель на реализацию `IStreamWriter`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) = 0;

    /// \~english @brief Associates both a stream reader and a stream writer with the named pipe.
    ///     This allows bidirectional communication through the pipe using the provided interfaces.
    /// \~russian @brief Связывает как читателя, так и писателя потока с именованным каналом.
    ///     Это позволяет осуществлять двунаправленную связь через канал с использованием предоставленных интерфейсов.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader A shared pointer to an `IStreamReader` implementation.
    /// \~russian @param reader Умный указатель на реализацию `IStreamReader`.
    /// \~english @param writer A shared pointer to an `IStreamWriter` implementation.
    /// \~russian @param writer Умный указатель на реализацию `IStreamWriter`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader, IStreamWriter::TSharedPtr const &writer) = 0;
};


} // namespace iridium::io


#endif // HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
