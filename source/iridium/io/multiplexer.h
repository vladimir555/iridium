// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h"

#include "event.h"

#include <chrono>


namespace iridium {
namespace io {


/// \~english @brief Interface for an I/O multiplexer.
/// \~english @details An I/O multiplexer monitors multiple I/O streams (e.g., sockets, file descriptors)
/// \~english and waits for one or more of them to become ready for I/O operations (like read or write).
/// \~english This is a central component in scalable, non-blocking I/O architectures.
/// \~russian @brief Интерфейс для мультиплексора ввода-вывода.
/// \~russian @details Мультиплексор ввода-вывода отслеживает несколько потоков ввода-вывода (например, сокеты, файловые дескрипторы)
/// \~russian и ожидает, пока один или несколько из них станут готовы для выполнения операций ввода-вывода (например, чтения или записи).
/// \~russian Это центральный компонент в масштабируемых, неблокирующих архитектурах ввода-вывода.
class IMultiplexer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IMultiplexer)

    /// \~english @brief Default timeout for waiting for events.
    /// \~russian @brief Таймаут по умолчанию для ожидания событий.
    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    /// \~english @brief Waits for I/O events to occur on subscribed streams.
    /// \~english @details This is a blocking call that waits until at least one event occurs or a timeout expires.
    /// \~english @return A list of events that have occurred. The list may be empty if the wait timed out.
    /// \~russian @brief Ожидает наступления событий ввода-вывода на подписанных потоках.
    /// \~russian @details Это блокирующий вызов, который ожидает, пока не произойдет хотя бы одно событие или не истечет таймаут.
    /// \~russian @return Список произошедших событий. Список может быть пустым, если ожидание завершилось по таймауту.
    virtual std::list<Event::TSharedPtr> waitEvents() = 0;

    /// \~english @brief Subscribes a stream to the multiplexer for event monitoring.
    /// \~russian @brief Подписывает поток на мультиплексор для отслеживания событий.
    /// \~english @param stream The stream to be monitored.
    /// \~russian @param stream Поток для отслеживания.
    virtual void subscribe  (IStream::TSharedPtr const &stream) = 0;

    /// \~english @brief Unsubscribes a stream from the multiplexer.
    /// \~russian @brief Отписывает поток от мультиплексора.
    /// \~english @param stream The stream to stop monitoring.
    /// \~russian @param stream Поток, отслеживание которого нужно прекратить.
    virtual void unsubscribe(IStream::TSharedPtr const &stream) = 0;

    /// \~english @brief Wakes up the multiplexer from its `waitEvents` call, possibly with a specific event.
    /// \~english @details This is used to interrupt the waiting and force it to process a new event, often from another thread.
    /// \~russian @brief "Пробуждает" мультиплексор из его вызова `waitEvents`, возможно, с определенным событием.
    /// \~russian @details Используется для прерывания ожидания и принудительной обработки нового события, часто из другого потока.
    /// \~english @param event The event to process.
    /// \~russian @param event Событие для обработки.
    virtual void wake(Event::TSharedPtr const &event) = 0;
};


} // io
} // iridium


#endif // HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
