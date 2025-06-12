// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the interface for an I/O event multiplexer.
/// \~russian @brief Определяет интерфейс для мультиплексора событий ввода-вывода.
#ifndef HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h" // For IInitializable
#include "iridium/smart_ptr.h"           // For DEFINE_INTERFACE and TSharedPtr (used by Event::TSharedPtr)

#include "event.h"    // For Event::TSharedPtr
#include "stream.h"   // For IStream::TSharedPtr

#include <chrono>     // For std::chrono::seconds
#include <list>       // For std::list


namespace iridium {
namespace io {


/// \~english @brief Interface for an I/O event multiplexer.
///     An I/O multiplexer allows a single thread to monitor multiple I/O streams (e.g., sockets, pipes)
///     for events such as readability or writability. This is fundamental for building scalable,
///     non-blocking I/O applications. It inherits from `IInitializable`, suggesting that
///     multiplexer instances may require an initialization phase.
///     The `todo` comment `public IAsyncQueuePopper<IEvent::TSharedPtr>` suggests a potential
///     design consideration for this interface to also act as a queue from which events can be popped.
/// \~russian @brief Интерфейс для мультиплексора событий ввода-вывода.
///     Мультиплексор ввода-вывода позволяет одному потоку отслеживать несколько потоков ввода-вывода
///     (например, сокеты, каналы) на предмет событий, таких как готовность к чтению или записи.
///     Это фундаментальный компонент для создания масштабируемых неблокирующих приложений ввода-вывода.
///     Наследуется от `IInitializable`, что предполагает, что экземпляры мультиплексора могут
///     требовать фазы инициализации.
///     Комментарий `todo: public IAsyncQueuePopper<IEvent::TSharedPtr>` предполагает потенциальное
///     проектное решение, чтобы этот интерфейс также выступал в роли очереди, из которой можно извлекать события.
class IMultiplexer: public pattern::IInitializable {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IMultiplexer)

    /// \~english @brief Default timeout value for waiting for events, in seconds.
    ///     Implementations may use this as a default if no specific timeout is provided to `waitEvents`
    ///     (though the current `waitEvents` signature does not take a timeout).
    /// \~russian @brief Значение тайм-аута по умолчанию для ожидания событий, в секундах.
    ///     Реализации могут использовать это значение по умолчанию, если в `waitEvents` не указан конкретный тайм-аут
    ///     (хотя текущая сигнатура `waitEvents` не принимает тайм-аут).
    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    /// \~english @brief Waits for I/O events to occur on any of the subscribed streams.
    ///     This method typically blocks until at least one event occurs or a timeout expires (though no timeout parameter is currently specified).
    ///     It is guaranteed not to throw exceptions (`no throw`).
    /// \~russian @brief Ожидает возникновения событий ввода-вывода на любом из подписанных потоков.
    ///     Этот метод обычно блокируется до тех пор, пока не произойдет хотя бы одно событие или не истечет тайм-аут (хотя параметр тайм-аута в настоящее время не указан).
    ///     Гарантированно не выбрасывает исключений (`no throw`).
    /// \~english @return A list of shared pointers to `Event` objects representing the I/O events that occurred.
    ///     The list may be empty if the wait timed out without any events.
    /// \~russian @return Список умных указателей на объекты `Event`, представляющие произошедшие события ввода-вывода.
    ///     Список может быть пустым, если ожидание завершилось по тайм-ауту без каких-либо событий.
    virtual std::list<Event::TSharedPtr> waitEvents() = 0;

    /// \~english @brief Subscribes an I/O stream to this multiplexer for event monitoring.
    ///     Once a stream is subscribed, the multiplexer will watch for I/O events on it (e.g., data available for reading).
    ///     The `todo` comment "subscribe for IStreamReader and IStreamWriter" suggests that future enhancements
    ///     might involve more granular subscriptions based on reader/writer capabilities.
    /// \~russian @brief Подписывает поток ввода-вывода на этот мультиплексор для отслеживания событий.
    ///     После подписки потока мультиплексор будет отслеживать события ввода-вывода на нем (например, доступность данных для чтения).
    ///     Комментарий `todo: subscribe for IStreamReader and IStreamWriter` предполагает, что будущие улучшения
    ///     могут включать более гранулированные подписки на основе возможностей чтения/записи.
    /// \~english @param stream A shared pointer to the `IStream` to be monitored.
    /// \~russian @param stream Умный указатель на `IStream`, который необходимо отслеживать.
    virtual void subscribe  (IStream::TSharedPtr const &stream) = 0;

    /// \~english @brief Unsubscribes an I/O stream from this multiplexer.
    ///     The multiplexer will stop monitoring events for this stream.
    ///     The comment "finalize and unsubscribe stream" suggests this might also trigger cleanup or finalization of the stream itself.
    /// \~russian @brief Отписывает поток ввода-вывода от этого мультиплексора.
    ///     Мультиплексор прекратит отслеживание событий для этого потока.
    ///     Комментарий "finalize and unsubscribe stream" предполагает, что это также может инициировать очистку или финализацию самого потока.
    /// \~english @param stream A shared pointer to the `IStream` to be removed from monitoring.
    /// \~russian @param stream Умный указатель на `IStream`, который необходимо удалить из отслеживания.
    virtual void unsubscribe(IStream::TSharedPtr const &stream) = 0;
};


} // io
} // iridium


#endif // HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
