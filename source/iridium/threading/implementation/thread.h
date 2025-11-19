// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CThread` class, a concrete implementation of the `IThread` interface.
///     This class manages a `std::thread` and interacts with an `IRunnable` object to perform work.
/// \~russian @file
/// @brief Определяет класс `CThread`, конкретную реализацию интерфейса `IThread`.
///     Этот класс управляет `std::thread` и взаимодействует с объектом `IRunnable` для выполнения работы.

#ifndef HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
#define HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA


#include "iridium/smart_ptr.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"

#include "async_queue.h"

#include <thread>
#include <atomic>
#include <string>
#include <chrono>


namespace iridium::threading::implementation {


class CThread:
    public IThread,
    public pattern::NonCopyable,
    public pattern::NonMovable
{
public:
    /// \~english @brief Defines standard factory methods (e.g., `create()`) and smart pointer typedefs for `CThread`.
    /// \~russian @brief Определяет стандартные фабричные методы (например, `create()`) и typedef-ы умных указателей для `CThread`.
    DEFINE_CREATE(CThread)

    /// \~english @brief Default timeout for thread operations, e.g., waiting for start/stop.
    /// \~russian @brief Тайм-аут по умолчанию для операций потока, например, ожидания запуска/остановки.
    static std::chrono::seconds const DEFAULT_TIMEOUT;
    CThread(
        std::string const &name,
        IRunnable::TSharedPtr const &runnuble,
        std::chrono::nanoseconds const &timeout = DEFAULT_TIMEOUT);
    virtual ~CThread();

    void initialize() override;
    void finalize() override;

    bool operator < (IThread const &thread) const override;
    std::string getName() const override;

protected:
    static void run(
        std::string             const &name,
        IRunnable::TSharedPtr   const &runnuble,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_start,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_stop,
        std::atomic<bool> *     const  is_running
    );
    std::string const
        m_name;
    IRunnable::TSharedPtr
        m_runnuble;
    std::shared_ptr<std::thread>
        m_thread;
private:
    /// \~english @brief Checks an error queue for a message, waiting up to the thread's timeout.
    /// \~russian @brief Проверяет очередь ошибок на наличие сообщения, ожидая в течение тайм-аута потока.
    /// \~english @param error_queue The queue to check for error messages.
    /// \~russian @param error_queue Очередь для проверки сообщений об ошибках.
    /// \~english @return The error message string if one is received, or an empty string if no error or timeout.
    /// \~russian @return Строка сообщения об ошибке, если получена, или пустая строка, если нет ошибки или истек тайм-аут.
    std::string checkErrorQueue(IAsyncQueuePopper<std::string>::TSharedPtr const &error_queue);

    /// \~english @brief Atomic flag indicating whether the thread's `run` method should continue execution. Controlled by `finalize()`.
    /// \~russian @brief Атомарный флаг, указывающий, должен ли метод `run` потока продолжать выполнение. Управляется `finalize()`.
    std::atomic<bool>
        m_is_running;
    /// \~english @brief Queue used by the `run` static method to signal startup status/errors back to `initialize()`.
    /// \~russian @brief Очередь, используемая статическим методом `run` для передачи статуса/ошибок запуска обратно в `initialize()`.
    IAsyncQueue<std::string>::TSharedPtr
        m_error_queue_start;
    /// \~english @brief Queue used by the `run` static method to signal stop status/errors back to `finalize()`.
    /// \~russian @brief Очередь, используемая статическим методом `run` для передачи статуса/ошибок остановки обратно в `finalize()`.
    IAsyncQueue<std::string>::TSharedPtr
        m_error_queue_stop;
    /// \~english @brief Timeout duration for operations like waiting for thread start/stop signals.
    /// \~russian @brief Продолжительность тайм-аута для операций, таких как ожидание сигналов запуска/остановки потока.
    std::chrono::nanoseconds const
        m_timeout;
};


} // iridium::threading::implementation


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
