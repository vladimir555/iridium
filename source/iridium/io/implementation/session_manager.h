// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
#define HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453


#include "iridium/io/context.h"
#include "iridium/io/context_manager.h"
#include "iridium/io/session_manager.h"
#include "iridium/io/multiplexer.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>
#include <unordered_set>
#include <mutex>


namespace iridium {
namespace io {
namespace implementation {


// The commented-out section below appears to be design notes or examples for different I/O models (kevent, epoll, iocp)
// and how they might interact with session and stream states. It outlines potential event flows and state transitions.
// todo:
// fsm: kevent, epoll, iocp
// session: fsm state + pipes
// stream statuses:
// open,    begin   -> fsm
// open,    end     -> fsm -> protocol -> add, rm, modify pipes
// read,    begin   -> fsm.
// read,    end     -> fsm -> protocol
// write,   begin   -> fsm
// write,   end     -> fsm -> protocol
// close,   begin   -> fsm
// close,   end     -> fsm -> protocol

// example(kevent or epoll):
// stream -> manage -> event(open, begin) -> multiplexer(subscribe) +
// worker(initialize) -> event(open, end) ->
// worker(protocol) -> pipe(event(read, begin)) ->
// worker(read) -> event(read, end) -> if read size > 0 then repeat event(read, begin) + worker(protocol) -> context still valid, no event
// ...
// kevent -> event(read, begin) -> repeat worker(read) or event(timeout, unknown) -> worker(protocol) ->
// ...
// worker(protocol) -> not valid context -> event(close, begin) ->
// unsubscribe ->
// kevent -> event(close, end) ->
// worker(finalize + remove context)

// example(iocp):
// stream -> manage -> event(open, begin) ->
// worker(initialize) -> event(open, end) ->
// worker(protocol) -> pipe(event(read, begin)) ->
// worker(read) -> none
// iocp -> event(read, end) -> if size > 0 then repeat + worker(protocol) ->
// worker(protocol) -> not valid context -> event(close, begin) ->


/// \~english @brief Concrete implementation of `ISessionManager`.
///     Manages I/O sessions, typically for network connections. It integrates a multiplexer for handling multiple streams,
///     a context manager for associating state with streams, and worker threads for processing events and I/O operations.
/// \~russian @brief Конкретная реализация `ISessionManager`.
///     Управляет сеансами ввода-вывода, обычно для сетевых подключений. Интегрирует мультиплексор для обработки нескольких потоков,
///     менеджер контекстов для связывания состояния с потоками и рабочие потоки для обработки событий и операций ввода-вывода.
class CSessionManager: public ISessionManager {
public:
    DEFINE_IMPLEMENTATION(CSessionManager);
    
    /// \~english @brief Constructs a `CSessionManager` instance.
    ///     Initializes internal components like the multiplexer, context manager, and context worker.
    /// \~russian @brief Конструирует экземпляр `CSessionManager`.
    ///     Инициализирует внутренние компоненты, такие как мультиплексор, менеджер контекстов и обработчик контекстов.
    CSessionManager();

    /// \~english @brief Initializes the session manager and its components.
    ///     This typically starts worker threads for event processing and multiplexing.
    /// \~russian @brief Инициализирует менеджер сессий и его компоненты.
    ///     Обычно это запускает рабочие потоки для обработки событий и мультиплексирования.
    /// \~english @throws iridium::Exception or derived classes if initialization of any component fails.
    /// \~russian @throws iridium::Exception или производные классы, если инициализация какого-либо компонента не удалась.
    void initialize() override;

    /// \~english @brief Finalizes the session manager and its components.
    ///     This typically stops worker threads and cleans up resources.
    /// \~russian @brief Завершает работу менеджера сессий и его компонентов.
    ///     Обычно это останавливает рабочие потоки и очищает ресурсы.
    /// \~english @throws iridium::Exception or derived classes if finalization of any component fails.
    /// \~russian @throws iridium::Exception или производные классы, если завершение работы какого-либо компонента не удалось.
    void finalize() override;
    
    /// \~english @brief Starts managing a new stream with a given protocol.
    ///     Creates a context for the stream and protocol via the context manager,
    ///     then pushes an `Event::NEW_STREAM` to the context worker to begin processing.
    /// \~russian @brief Начинает управление новым потоком с заданным протоколом.
    ///     Создает контекст для потока и протокола через менеджер контекстов,
    ///     затем помещает `Event::NEW_STREAM` в обработчик контекстов для начала обработки.
    /// \~english @param stream The stream port to manage.
    /// \~russian @param stream Порт потока для управления.
    /// \~english @param protocol The protocol to use for communication over the stream.
    /// \~russian @param protocol Протокол, используемый для обмена данными через поток.
    void manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) override;

private:
    /// \~english @brief Typedef for a worker that processes events (`Event::TSharedPtr`).
    /// \~russian @brief Typedef для обработчика, который обрабатывает события (`Event::TSharedPtr`).
    typedef threading::IWorker<Event::TSharedPtr> IContextWorker;

    /// \~english @brief Runnable handler for the multiplexer thread.
    ///     This class is responsible for running the multiplexer loop, waiting for I/O events,
    ///     and dispatching them to the context worker.
    /// \~russian @brief Исполняемый обработчик для потока мультиплексора.
    ///     Этот класс отвечает за запуск цикла мультиплексора, ожидание событий ввода-вывода
    ///     и их отправку обработчику контекстов.
    class CMultiplexerThreadHandler: public iridium::threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CMultiplexerThreadHandler)
        
        /// \~english @brief Constructs a `CMultiplexerThreadHandler`.
        /// \~russian @brief Конструирует `CMultiplexerThreadHandler`.
        /// \~english @param context_worker Worker to process events generated by the multiplexer.
        /// \~russian @param context_worker Обработчик для обработки событий, сгенерированных мультиплексором.
        /// \~english @param context_manager Manager for contexts, used to check for outdated streams.
        /// \~russian @param context_manager Менеджер контекстов, используемый для проверки устаревших потоков.
        /// \~english @param multiplexer The I/O multiplexer instance.
        /// \~russian @param multiplexer Экземпляр мультиплексора ввода-вывода.
        CMultiplexerThreadHandler(
            IContextWorker::TSharedPtr  const &context_worker,
            IContextManager::TSharedPtr const &context_manager,
            IMultiplexer::TSharedPtr    const &multiplexer
        );

        /// \~english @brief Initializes the handler. (Currently a no-op).
        /// \~russian @brief Инициализирует обработчик. (В настоящее время не выполняет никаких действий).
        void initialize() override;
        /// \~english @brief Finalizes the handler. (Currently a no-op).
        /// \~russian @brief Завершает работу обработчика. (В настоящее время не выполняет никаких действий).
        void finalize() override;

        /// \~english @brief The main run loop for the multiplexer thread.
        ///     Continuously calls `multiplexer->wait()` and `context_manager->checkOutdatedStreams()`,
        ///     pushing any resulting events to the `context_worker`.
        /// \~russian @brief Основной цикл выполнения для потока мультиплексора.
        ///     Непрерывно вызывает `multiplexer->wait()` и `context_manager->checkOutdatedStreams()`,
        ///     помещая все результирующие события в `context_worker`.
        /// \~english @param is_running Atomic boolean to control the loop's execution. The loop continues as long as `is_running` is true.
        /// \~russian @param is_running Атомарный булев флаг для управления выполнением цикла. Цикл продолжается, пока `is_running` истинно.
        void run(std::atomic<bool> &is_running) override;
    private:
        /// \~english @brief Worker to process events from the multiplexer or outdated stream checks.
        /// \~russian @brief Обработчик для обработки событий от мультиплексора или проверок устаревших потоков.
        IContextWorker::TSharedPtr  m_context_worker;
        /// \~english @brief Context manager used to check for outdated streams.
        /// \~russian @brief Менеджер контекстов, используемый для проверки устаревших потоков.
        IContextManager::TSharedPtr m_context_manager;
        /// \~english @brief The I/O multiplexer.
        /// \~russian @brief Мультиплексор ввода-вывода.
        IMultiplexer::TSharedPtr    m_multiplexer;
    };

    /// \~english @brief Runnable handler for repeating events, potentially for retries or scheduled tasks.
    ///     (The current implementation of `run` is empty, suggesting it's a placeholder or its logic is elsewhere).
    /// \~russian @brief Исполняемый обработчик для повторения событий, возможно, для повторных попыток или запланированных задач.
    ///     (Текущая реализация `run` пуста, что предполагает, что это заглушка или ее логика находится в другом месте).
    class CEventRepeaterHandler: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CEventRepeaterHandler)
        
        /// \~english @brief Constructs an `CEventRepeaterHandler`.
        /// \~russian @brief Конструирует `CEventRepeaterHandler`.
        /// \~english @param context_worker Worker to which repeated events would be sent.
        /// \~russian @param context_worker Обработчик, которому будут отправляться повторенные события.
        CEventRepeaterHandler(IContextWorker::TSharedPtr const &context_worker);

        /// \~english @brief Initializes the handler. (Currently a no-op).
        /// \~russian @brief Инициализирует обработчик. (В настоящее время не выполняет никаких действий).
        void initialize() override;
        /// \~english @brief Finalizes the handler. (Currently a no-op).
        /// \~russian @brief Завершает работу обработчика. (В настоящее время не выполняет никаких действий).
        void finalize() override;

        /// \~english @brief The main run loop for the event repeater thread. (Currently empty).
        /// \~russian @brief Основной цикл выполнения для потока повторителя событий. (В настоящее время пуст).
        /// \~english @param is_running Atomic boolean to control the loop's execution.
        /// \~russian @param is_running Атомарный булев флаг для управления выполнением цикла.
        void run(std::atomic<bool> &is_running) override;
    private:
        /// \~english @brief Worker to which repeated events would be sent.
        /// \~russian @brief Обработчик, которому будут отправляться повторенные события.
        IContextWorker::TSharedPtr
            m_context_worker;
    };

    /// \~english @brief Handler for processing events within a context worker.
    ///     This class implements the `IContextWorker::IHandler` interface, providing the logic
    ///     to acquire, update/transmit on, and release contexts based on incoming events.
    /// \~russian @brief Обработчик для обработки событий в рамках обработчика контекстов.
    ///     Этот класс реализует интерфейс `IContextWorker::IHandler`, предоставляя логику
    ///     для получения, обновления/передачи и освобождения контекстов на основе входящих событий.
    class CContextWorkerHandler: public IContextWorker::IHandler {
    public:
        DEFINE_IMPLEMENTATION(CContextWorkerHandler)
        /// \~english @brief Constructs a `CContextWorkerHandler`.
        /// \~russian @brief Конструирует `CContextWorkerHandler`.
        /// \~english @param context_manager Manager for acquiring and releasing contexts.
        /// \~russian @param context_manager Менеджер для получения и освобождения контекстов.
        /// \~english @param multiplexer Multiplexer for managing stream subscriptions (e.g., after context removal).
        /// \~russian @param multiplexer Мультиплексор для управления подписками на потоки (например, после удаления контекста).
        CContextWorkerHandler(IContextManager::TSharedPtr const &context_manager, IMultiplexer::TSharedPtr const &multiplexer);
        /// \~english @brief Initializes the handler. (Currently a no-op).
        /// \~russian @brief Инициализирует обработчик. (В настоящее время не выполняет никаких действий).
        void initialize() override;
        /// \~english @brief Finalizes the handler. (Currently a no-op).
        /// \~russian @brief Завершает работу обработчика. (В настоящее время не выполняет никаких действий).
        void finalize() override;
        /// \~english @brief Handles a batch of input events.
        ///     For each event, it acquires the relevant context, performs actions (update, transmit, or remove)
        ///     based on the event type, and then releases the context.
        ///     It collects and returns any events generated during this processing (e.g., for further dispatch).
        /// \~russian @brief Обрабатывает пакет входных событий.
        ///     Для каждого события он получает соответствующий контекст, выполняет действия (обновление, передача или удаление)
        ///     в зависимости от типа события, а затем освобождает контекст.
        ///     Собирает и возвращает любые события, сгенерированные во время этой обработки (например, для дальнейшей отправки).
        /// \~english @param events A list of events to handle.
        /// \~russian @param events Список событий для обработки.
        /// \~english @return A list of output events generated during handling.
        /// \~russian @return Список выходных событий, сгенерированных во время обработки.
        IContextWorker::TOutputItems handle(IContextWorker::TInputItems const &events) override;
    private:
        /// \~english @brief Context manager for acquiring and releasing contexts.
        /// \~russian @brief Менеджер контекстов для получения и освобождения контекстов.
        IContextManager::TSharedPtr
            m_context_manager;
        /// \~english @brief Multiplexer, used here mainly to unsubscribe streams when contexts are closed.
        /// \~russian @brief Мультиплексор, используемый здесь в основном для отписки от потоков при закрытии контекстов.
        IMultiplexer::TSharedPtr
            m_multiplexer;
    };

    /// \~english @brief The I/O multiplexer (e.g., epoll, select) used to wait for events on multiple streams.
    /// \~russian @brief Мультиплексор ввода-вывода (например, epoll, select), используемый для ожидания событий на нескольких потоках.
    IMultiplexer::TSharedPtr
        m_multiplexer;
    /// \~english @brief Manages all active contexts, associating them with streams.
    /// \~russian @brief Управляет всеми активными контекстами, связывая их с потоками.
    IContextManager::TSharedPtr
        m_context_manager;
    /// \~english @brief Manages pipes within contexts (though not directly used in `CSessionManager` methods, it's likely passed to or used by `IContext` implementations).
    /// \~russian @brief Управляет каналами (pipes) в контекстах (хотя напрямую не используется в методах `CSessionManager`, вероятно, передается или используется реализациями `IContext`).
    IPipeManager::TSharedPtr
        m_pipe_manager; // Note: This member is initialized but not directly used in the provided CSessionManager code. It might be intended for future use or used by objects CSessionManager creates/manages.
    /// \~english @brief Worker thread/queue for processing context-related events asynchronously.
    /// \~russian @brief Рабочий поток/очередь для асинхронной обработки событий, связанных с контекстом.
    IContextWorker::TSharedPtr
        m_context_worker;
    /// \~english @brief Thread that runs the multiplexer loop (`CMultiplexerThreadHandler`).
    /// \~russian @brief Поток, выполняющий цикл мультиплексора (`CMultiplexerThreadHandler`).
    threading::IThread::TSharedPtr
        m_multiplexer_thread;
    /// \~english @brief Thread that runs the event repeater logic (`CEventRepeaterHandler`).
    /// \~russian @brief Поток, выполняющий логику повторителя событий (`CEventRepeaterHandler`).
    threading::IThread::TSharedPtr
        m_event_repeater_thread;
};



} // implementation
} // io
} // iridium


#endif // HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
