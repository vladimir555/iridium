// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
#define HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/convertion/convert.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/worker.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstring>
#include <array>
#include <atomic>
#include <mutex>

#include <sys/event.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


/// \~english @brief FreeBSD-specific implementation of `IMultiplexer` using `kqueue`.
///     This class allows monitoring multiple file descriptors (streams) for I/O events.
///     It uses internal pipes to handle asynchronous subscription and unsubscription requests.
///     Operations are synchronized using a mutex.
/// \~russian @brief Реализация `IMultiplexer` для FreeBSD, использующая `kqueue`.
///     Этот класс позволяет отслеживать несколько файловых дескрипторов (потоков) на предмет событий ввода-вывода.
///     Он использует внутренние каналы (pipes) для обработки асинхронных запросов на подписку и отписку.
///     Операции синхронизируются с использованием мьютекса.
class CMultiplexer: public IMultiplexer, public threading::Synchronized<std::mutex> {
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    /// \~english @brief Constructs a `CMultiplexer` instance.
    /// \~russian @brief Конструирует экземпляр `CMultiplexer`.
    /// \~english @param timeout The maximum time to wait for events in `waitEvents()`. Defaults to `DEFAULT_WAITING_TIMEOUT`.
    /// \~russian @param timeout Максимальное время ожидания событий в `waitEvents()`. По умолчанию `DEFAULT_WAITING_TIMEOUT`.
    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT);

    /// \~english @brief Initializes the multiplexer.
    ///     Creates the `kqueue` instance and sets up internal pipes for managing subscriptions.
    ///     Initializes signal handling for `SIGPIPE`.
    /// \~russian @brief Инициализирует мультиплексор.
    ///     Создает экземпляр `kqueue` и настраивает внутренние каналы для управления подписками.
    ///     Инициализирует обработку сигнала `SIGPIPE`.
    /// \~english @throws std::runtime_error if `kqueue` creation or pipe registration fails.
    /// \~russian @throws std::runtime_error, если создание `kqueue` или регистрация канала не удалась.
    void initialize()   override;

    /// \~english @brief Finalizes the multiplexer.
    ///     Closes the `kqueue` instance and the internal pipes.
    /// \~russian @brief Завершает работу мультиплексора.
    ///     Закрывает экземпляр `kqueue` и внутренние каналы.
    void finalize()     override;

    /// \~english @brief Waits for I/O events on subscribed streams or until a timeout occurs.
    ///     Uses `kevent()` to poll for events. Handles events from internal pipes for subscription changes.
    /// \~russian @brief Ожидает событий ввода-вывода на подписанных потоках или до истечения тайм-аута.
    ///     Использует `kevent()` для опроса событий. Обрабатывает события из внутренних каналов для изменений подписки.
    /// \~english @return A list of `Event::TSharedPtr` representing the detected I/O events.
    /// \~russian @return Список `Event::TSharedPtr`, представляющий обнаруженные события ввода-вывода.
    /// \~english @throws std::runtime_error if `kevent()` fails.
    /// \~russian @throws std::runtime_error, если `kevent()` завершается с ошибкой.
    std::list<Event::TSharedPtr> waitEvents() override;

    /// \~english @brief Subscribes a stream to I/O event monitoring.
    ///     Sends a message via an internal pipe to the `waitEvents()` loop to register the stream's file descriptors with `kqueue`.
    /// \~russian @brief Подписывает поток на мониторинг событий ввода-вывода.
    ///     Отправляет сообщение через внутренний канал в цикл `waitEvents()` для регистрации файловых дескрипторов потока в `kqueue`.
    /// \~english @param stream The stream to subscribe.
    /// \~russian @param stream Поток для подписки.
    /// \~english @throws std::runtime_error if writing to the internal pipe fails.
    /// \~russian @throws std::runtime_error, если запись во внутренний канал не удалась.
    void subscribe  (IStream::TSharedPtr const &stream) override;

    /// \~english @brief Unsubscribes a stream from I/O event monitoring.
    ///     Sends a message via an internal pipe to the `waitEvents()` loop to deregister the stream's file descriptors from `kqueue`.
    /// \~russian @brief Отписывает поток от мониторинга событий ввода-вывода.
    ///     Отправляет сообщение через внутренний канал в цикл `waitEvents()` для отмены регистрации файловых дескрипторов потока из `kqueue`.
    /// \~english @param stream The stream to unsubscribe.
    /// \~russian @param stream Поток для отписки.
    /// \~english @throws std::runtime_error if writing to the internal pipe fails.
    /// \~russian @throws std::runtime_error, если запись во внутренний канал не удалась.
    void unsubscribe(IStream::TSharedPtr const &stream) override;

private:
    /// \~english @brief Default limit for the number of events to retrieve from `kevent` in one call.
    /// \~russian @brief Ограничение по умолчанию на количество событий, извлекаемых из `kevent` за один вызов.
    static size_t const DEFAULT_EVENTS_LIMIT = 4;

    /// \~english @brief Asserts that a system call result indicates success.
    /// \~russian @brief Проверяет, что результат системного вызова указывает на успех.
    /// \~english @tparam T The type of the result.
    /// \~russian @tparam T Тип результата.
    /// \~english @param result The result of a system call.
    /// \~russian @param result Результат системного вызова.
    /// \~english @param message A message to include in the exception if an error occurred.
    /// \~russian @param message Сообщение для включения в исключение в случае ошибки.
    /// \~english @return The `result` if it indicates success.
    /// \~russian @return `result`, если он указывает на успех.
    /// \~english @throws std::runtime_error if `result` indicates an error (typically < 0).
    /// \~russian @throws std::runtime_error, если `result` указывает на ошибку (обычно < 0).
    template<typename T>
    static T assertOK(T const &result, std::string const &message);

    /// \~english @brief Signal handler, typically for `SIGPIPE`.
    ///     Currently, it's a no-op, but it's registered to prevent default termination on `SIGPIPE`.
    /// \~russian @brief Обработчик сигналов, обычно для `SIGPIPE`.
    ///     В настоящее время это пустая операция, но он зарегистрирован для предотвращения завершения по умолчанию при `SIGPIPE`.
    /// \~english @param signal The signal number.
    /// \~russian @param signal Номер сигнала.
    static void handleSignal(int signal);

    /// \~english @brief Registers a new pipe and configures it for non-blocking I/O.
    ///     Used to create internal communication channels for managing subscriptions.
    /// \~russian @brief Регистрирует новый канал (pipe) и настраивает его для неблокирующего ввода-вывода.
    ///     Используется для создания внутренних каналов связи для управления подписками.
    /// \~english @return An array of two integers: the read end file descriptor and the write end file descriptor of the pipe.
    /// \~russian @return Массив из двух целых чисел: файловый дескриптор конца для чтения и файловый дескриптор конца для записи канала.
    /// \~english @throws std::runtime_error if `pipe()` or `fcntl()` calls fail.
    /// \~russian @throws std::runtime_error, если вызовы `pipe()` или `fcntl()` завершаются с ошибкой.
    std::array<int, 2> registerPipe();

    /// \~english @brief Timeout structure for `kevent()`. Derived from the constructor's timeout parameter.
    /// \~russian @brief Структура тайм-аута для `kevent()`. Производная от параметра тайм-аута конструктора.
    struct timespec m_timeout;

    /// \~english @brief Vector to store events triggered by `kevent()`.
    /// \~russian @brief Вектор для хранения событий, вызванных `kevent()`.
    std::vector<struct kevent> m_triggered_events;

    /// \~english @brief The file descriptor for the `kqueue` instance. Atomic for safe access if needed, though typically managed under lock.
    /// \~russian @brief Файловый дескриптор для экземпляра `kqueue`. Атомарный для безопасного доступа, если это необходимо, хотя обычно управляется под блокировкой.
    std::atomic<int>    m_kqueue;
    /// \~english @brief Internal pipe used to signal the `waitEvents()` loop to add new stream subscriptions. `m_pipe_add[0]` is read end, `m_pipe_add[1]` is write end.
    /// \~russian @brief Внутренний канал, используемый для сигнализации циклу `waitEvents()` о добавлении новых подписок на потоки. `m_pipe_add[0]` - конец для чтения, `m_pipe_add[1]` - конец для записи.
    std::array<int, 2>  m_pipe_add;
    /// \~english @brief Internal pipe used to signal the `waitEvents()` loop to remove stream subscriptions. `m_pipe_del[0]` is read end, `m_pipe_del[1]` is write end.
    /// \~russian @brief Внутренний канал, используемый для сигнализации циклу `waitEvents()` об удалении подписок на потоки. `m_pipe_del[0]` - конец для чтения, `m_pipe_del[1]` - конец для записи.
    std::array<int, 2>  m_pipe_del;

    /// \~english @brief Maps a file descriptor (uintptr_t) to its corresponding `IStream::TSharedPtr`.
    ///     Used to retrieve the stream object when an event occurs on a file descriptor.
    /// \~russian @brief Сопоставляет файловый дескриптор (uintptr_t) с соответствующим `IStream::TSharedPtr`.
    ///     Используется для извлечения объекта потока при возникновении события на файловом дескрипторе.
    std::unordered_map<uintptr_t, IStream::TSharedPtr> m_map_fd_stream;
};


template<typename T>
T CMultiplexer::assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " + std::strerror(errno) +
          ", code " + iridium::convertion::convert<std::string>(errno)); // ----->
    else
        return result; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
#endif // HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
