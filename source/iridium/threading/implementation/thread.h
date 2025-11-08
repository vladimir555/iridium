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


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
namespace threading {
namespace implementation {


/// \~english @brief Concrete implementation of the `IThread` interface.
///     Manages an actual `std::thread` to execute an `IRunnable` task.
///     This class is non-copyable and non-movable.
/// \~russian @brief Конкретная реализация интерфейса `IThread`.
///     Управляет фактическим `std::thread` для выполнения задачи `IRunnable`.
///     Этот класс не является копируемым и неперемещаемым.
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

    /// \~english @brief Constructs a `CThread` object.
    ///     The actual OS thread is not started until `initialize()` is called.
    /// \~russian @brief Конструирует объект `CThread`.
    ///     Фактический поток ОС не запускается до вызова `initialize()`.
    /// \~english @param name The name for the thread, used for identification and logging.
    /// \~russian @param name Имя потока, используемое для идентификации и логирования.
    /// \~english @param runnuble A shared pointer to an `IRunnable` object whose `run()` method will be executed by this thread.
    /// \~russian @param runnuble Умный указатель на объект `IRunnable`, метод `run()` которого будет выполнен этим потоком.
    /// \~english @param timeout Default timeout duration for operations like waiting for thread start/stop. Defaults to `DEFAULT_TIMEOUT`.
    /// \~russian @param timeout Продолжительность тайм-аута по умолчанию для операций, таких как ожидание запуска/остановки потока. По умолчанию `DEFAULT_TIMEOUT`.
    CThread(
        std::string const &name,
        IRunnable::TSharedPtr const &runnuble,
        std::chrono::nanoseconds const &timeout = DEFAULT_TIMEOUT);

    /// \~english @brief Destructor. Ensures the thread is finalized (joined) if it was running.
    /// \~russian @brief Деструктор. Гарантирует, что поток финализирован (присоединен), если он был запущен.
    virtual ~CThread();

    /// \~english @brief Initializes the thread, which includes starting the underlying `std::thread`
    ///     and waiting for it to signal successful startup or an error.
    /// \~russian @brief Инициализирует поток, что включает запуск базового `std::thread`
    ///     и ожидание сигнала об успешном запуске или ошибке.
    /// \~english @throws std::runtime_error if the thread fails to start within the timeout or if an error occurs during startup.
    /// \~russian @throws std::runtime_error если поток не запускается в течение тайм-аута или если во время запуска возникает ошибка.
    void initialize() override;

    /// \~english @brief Finalizes the thread. Signals the `IRunnable` to stop, then joins the underlying `std::thread`.
    ///     Waits for the thread to acknowledge shutdown or for an error.
    /// \~russian @brief Финализирует поток. Сигнализирует `IRunnable` об остановке, затем присоединяет базовый `std::thread`.
    ///     Ожидает подтверждения завершения работы потоком или ошибки.
    /// \~english @throws std::runtime_error if the thread fails to stop cleanly within the timeout or if an error occurs during shutdown.
    /// \~russian @throws std::runtime_error если поток не останавливается корректно в течение тайм-аута или если во время завершения работы возникает ошибка.
    void finalize() override;

    /// \~english @brief Compares this `CThread` object with another `IThread` object.
    ///     The comparison is based on the thread name.
    /// \~russian @brief Сравнивает этот объект `CThread` с другим объектом `IThread`.
    ///     Сравнение основано на имени потока.
    /// \~english @param thread The other `IThread` object to compare against.
    /// \~russian @param thread Другой объект `IThread` для сравнения.
    /// \~english @return True if this thread's name is lexicographically less than the other thread's name.
    /// \~russian @return True, если имя этого потока лексикографически меньше имени другого потока.
    bool operator < (IThread const &thread) const override;

    /// \~english @brief Gets the name of this thread.
    /// \~russian @brief Получает имя этого потока.
    /// \~english @return The name of the thread as a string.
    /// \~russian @return Имя потока в виде строки.
    std::string getName() const override;

protected:
    /// \~english @brief The static function executed by the `std::thread`.
    ///     It sets the thread name, calls the `run()` method of the `runnuble` object,
    ///     and uses queues to signal start/stop status and errors.
    /// \~russian @brief Статическая функция, выполняемая `std::thread`.
    ///     Устанавливает имя потока, вызывает метод `run()` объекта `runnuble`
    ///     и использует очереди для сигнализации статуса запуска/остановки и ошибок.
    /// \~english @param name The name to assign to the executing thread.
    /// \~russian @param name Имя, присваиваемое выполняющемуся потоку.
    /// \~english @param runnuble The `IRunnable` task to execute.
    /// \~russian @param runnuble Задача `IRunnable` для выполнения.
    /// \~english @param status_start Queue used to signal that the thread has started successfully or encountered an error during startup.
    /// \~russian @param status_start Очередь, используемая для сигнализации об успешном запуске потока или ошибке во время запуска.
    /// \~english @param status_stop Queue used to signal that the thread has stopped successfully or encountered an error during shutdown.
    /// \~russian @param status_stop Очередь, используемая для сигнализации об успешной остановке потока или ошибке во время завершения работы.
    /// \~english @param is_running Pointer to an atomic boolean flag that indicates if the thread's main loop should continue running.
    /// \~russian @param is_running Указатель на атомарный логический флаг, указывающий, должен ли основной цикл потока продолжать выполняться.
    static void run(
        std::string             const &name,
        IRunnable::TSharedPtr   const &runnuble,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_start,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_stop,
        std::atomic<bool> *     const  is_running
    );

    /// \~english @brief The name of this thread instance.
    /// \~russian @brief Имя этого экземпляра потока.
    std::string const               m_name;
    /// \~english @brief The `IRunnable` task this thread will execute.
    /// \~russian @brief Задача `IRunnable`, которую будет выполнять этот поток.
    IRunnable::TSharedPtr           m_runnuble;
    /// \~english @brief The underlying `std::thread` object.
    /// \~russian @brief Базовый объект `std::thread`.
    std::shared_ptr<std::thread>    m_thread;
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


} // implementation
} // threading
} // iridium


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
