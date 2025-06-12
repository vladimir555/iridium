// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
#define HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <unordered_map>
#include <sys/epoll.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


/// \~english @brief Linux-specific implementation of `IMultiplexer` using `epoll`.
///     This class allows monitoring multiple file descriptors (streams) for I/O events.
///     It uses an `eventfd` for asynchronous wake-up to handle subscription changes.
///     Operations that modify epoll subscriptions are queued and processed in the `waitEvents` loop.
/// \~russian @brief Реализация `IMultiplexer` для Linux, использующая `epoll`.
///     Этот класс позволяет отслеживать несколько файловых дескрипторов (потоков) на предмет событий ввода-вывода.
///     Он использует `eventfd` для асинхронного пробуждения для обработки изменений подписки.
///     Операции, изменяющие подписки epoll, помещаются в очередь и обрабатываются в цикле `waitEvents`.
class CMultiplexer:
    public IMultiplexer,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    /// \~english @brief Constructs a `CMultiplexer` instance.
    ///     Initializes queues for adding/deleting streams and sets the closing flag to false.
    /// \~russian @brief Конструирует экземпляр `CMultiplexer`.
    ///     Инициализирует очереди для добавления/удаления потоков и устанавливает флаг закрытия в false.
    CMultiplexer();

    /// \~english @brief Initializes the epoll multiplexer.
    ///     Creates an epoll instance and an eventfd for internal signaling.
    ///     The eventfd is added to the epoll set to wake up `epoll_wait`.
    /// \~russian @brief Инициализирует мультиплексор epoll.
    ///     Создает экземпляр epoll и eventfd для внутреннего сигнализирования.
    ///     Eventfd добавляется в набор epoll для пробуждения `epoll_wait`.
    /// \~english @throws std::runtime_error if epoll or eventfd creation fails.
    /// \~russian @throws std::runtime_error, если создание epoll или eventfd не удалось.
    void    initialize()   override;

    /// \~english @brief Finalizes the epoll multiplexer.
    ///     Sets a closing flag, signals the eventfd to wake up the `waitEvents` loop,
    ///     and then closes the epoll and eventfd file descriptors.
    /// \~russian @brief Завершает работу мультиплексора epoll.
    ///     Устанавливает флаг закрытия, сигнализирует eventfd для пробуждения цикла `waitEvents`,
    ///     а затем закрывает файловые дескрипторы epoll и eventfd.
    void    finalize()     override;

    /// \~english @brief Subscribes a stream for I/O event monitoring.
    ///     Adds the stream to an internal queue and signals the `eventfd` to process the subscription in the `waitEvents` loop.
    /// \~russian @brief Подписывает поток на мониторинг событий ввода-вывода.
    ///     Добавляет поток во внутреннюю очередь и сигнализирует `eventfd` для обработки подписки в цикле `waitEvents`.
    /// \~english @param stream The stream to subscribe.
    /// \~russian @param stream Поток для подписки.
    void    subscribe(IStream::TSharedPtr const &stream) override;

    /// \~english @brief Unsubscribes a stream from I/O event monitoring.
    ///     Adds the stream to an internal queue and signals the `eventfd` to process the unsubscription in the `waitEvents` loop.
    /// \~russian @brief Отписывает поток от мониторинга событий ввода-вывода.
    ///     Добавляет поток во внутреннюю очередь и сигнализирует `eventfd` для обработки отписки в цикле `waitEvents`.
    /// \~english @param stream The stream to unsubscribe.
    /// \~russian @param stream Поток для отписки.
    void  unsubscribe(IStream::TSharedPtr const &stream) override;

    /// \~english @brief Waits for I/O events on subscribed streams or until an internal signal occurs.
    ///     Processes any pending stream subscriptions/unsubscriptions before calling `epoll_wait`.
    ///     Translates epoll events into `iridium::io::Event` objects.
    /// \~russian @brief Ожидает событий ввода-вывода на подписанных потоках или до возникновения внутреннего сигнала.
    ///     Обрабатывает все ожидающие подписки/отписки потоков перед вызовом `epoll_wait`.
    ///     Преобразует события epoll в объекты `iridium::io::Event`.
    /// \~english @return A list of `Event::TSharedPtr` representing the detected I/O events.
    /// \~russian @return Список `Event::TSharedPtr`, представляющий обнаруженные события ввода-вывода.
    /// \~english @throws std::runtime_error if `epoll_wait` fails.
    /// \~russian @throws std::runtime_error, если `epoll_wait` завершается с ошибкой.
    std::list<Event::TSharedPtr> waitEvents() override;

private:
    /// \~english @brief Default limit for the number of events to retrieve from `epoll_wait` in one call.
    /// \~russian @brief Ограничение по умолчанию на количество событий, извлекаемых из `epoll_wait` за один вызов.
    static size_t const DEFAULT_EVENTS_COUNT_LIMIT = 2;

    /// \~english @brief Asserts that a system call result indicates success.
    /// \~russian @brief Проверяет, что результат системного вызова указывает на успех.
    /// \~english @param result The result of a system call.
    /// \~russian @param result Результат системного вызова.
    /// \~english @param message A message to include in the exception if an error occurred.
    /// \~russian @param message Сообщение для включения в исключение в случае ошибки.
    /// \~english @return The `result` if it indicates success.
    /// \~russian @return `result`, если он указывает на успех.
    /// \~english @throws std::runtime_error if `result` indicates an error (typically < 0).
    /// \~russian @throws std::runtime_error, если `result` указывает на ошибку (обычно < 0).
    static int assertOK(int const &result, std::string const &message);

    /// \~english @brief Internal helper to add a stream's file descriptors to the epoll set.
    ///     Called by `waitEvents` to process the `m_streams_to_add` queue.
    /// \~russian @brief Внутренний вспомогательный метод для добавления файловых дескрипторов потока в набор epoll.
    ///     Вызывается `waitEvents` для обработки очереди `m_streams_to_add`.
    /// \~english @param stream The stream to add to epoll monitoring.
    /// \~russian @param stream Поток для добавления в мониторинг epoll.
    void    addInternal(IStream::TSharedPtr const &stream);

    /// \~english @brief Internal helper to remove a stream's file descriptors from the epoll set.
    ///     Called by `waitEvents` to process the `m_streams_to_del` queue.
    /// \~russian @brief Внутренний вспомогательный метод для удаления файловых дескрипторов потока из набора epoll.
    ///     Вызывается `waitEvents` для обработки очереди `m_streams_to_del`.
    /// \~english @param stream The stream to remove from epoll monitoring.
    /// \~russian @param stream Поток для удаления из мониторинга epoll.
    void    delInternal(IStream::TSharedPtr const &stream);

    /// \~english @brief Maps a file descriptor (uintptr_t) to its corresponding `IStream::TSharedPtr`.
    ///     Used to retrieve the stream object when an event occurs on a file descriptor.
    /// \~russian @brief Сопоставляет файловый дескриптор (uintptr_t) с соответствующим `IStream::TSharedPtr`.
    ///     Используется для извлечения объекта потока при возникновении события на файловом дескрипторе.
    std::unordered_map<uintptr_t, IStream::TSharedPtr>
                        m_map_fd_stream;
    /// \~english @brief The file descriptor for the epoll instance. Atomic for safe initialization.
    /// \~russian @brief Файловый дескриптор для экземпляра epoll. Атомарный для безопасной инициализации.
    std::atomic<int>    m_epoll_fd;
    /// \~english @brief File descriptor for `eventfd`, used for internal signaling to wake up `epoll_wait`.
    /// \~russian @brief Файловый дескриптор для `eventfd`, используемый для внутреннего сигнализирования для пробуждения `epoll_wait`.
    int                 m_event_fd;
    
    /// \~english @brief Asynchronous queue for streams pending addition to epoll.
    /// \~russian @brief Асинхронная очередь для потоков, ожидающих добавления в epoll.
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_streams_to_add;
    /// \~english @brief Asynchronous queue for streams pending removal from epoll.
    /// \~russian @brief Асинхронная очередь для потоков, ожидающих удаления из epoll.
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_streams_to_del;
    
    /// \~english @brief Atomic flag indicating if the multiplexer is in the process of closing.
    ///     Used to prevent operations during finalization.
    /// \~russian @brief Атомарный флаг, указывающий, находится ли мультиплексор в процессе закрытия.
    ///     Используется для предотвращения операций во время завершения работы.
    std::atomic<bool> m_is_closing;
};


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
