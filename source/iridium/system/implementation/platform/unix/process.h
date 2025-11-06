#ifndef HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F
#define HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM

/// \~english @file
/// @brief Generic Unix platform-specific implementation for process management.
///     This header defines `CProcessStream`, which is the concrete class for handling
///     external processes on Unix-like systems (including Linux, macOS, FreeBSD, etc.,
///     which then alias this implementation). It uses traditional Unix mechanisms like
///     `fork`, `exec`, pipes for I/O redirection, and `waitpid` for state management.
/// \~russian @file
/// @brief Платформо-специфичная реализация управления процессами для общих Unix систем.
///     Этот заголовочный файл определяет `CProcessStream`, который является конкретным классом
///     для обработки внешних процессов в Unix-подобных системах (включая Linux, macOS, FreeBSD и др.,
///     которые затем используют эту реализацию через псевдонимы). Использует традиционные механизмы Unix,
///     такие как `fork`, `exec`, каналы (pipes) для перенаправления ввода-вывода и `waitpid` для управления состоянием.

#include "iridium/io/implementation/stream_port.h"
#include "iridium/system/process.h"
#include "iridium/threading/synchronized.h"

#include <string>
#include <sched.h> // For sched_yield, often used in process management utilities


namespace iridium {
namespace system {
namespace implementation {
namespace platform {
/// \~english @brief Contains Unix-specific implementations of system components, particularly for process stream management.
///     This namespace is aliased by specific Unix-like platforms (Linux, macOS, FreeBSD) to reuse this common implementation.
/// \~russian @brief Содержит Unix-специфичные реализации системных компонентов, в частности для управления потоками процессов.
///     Это пространство имен используется через псевдонимы специфичными Unix-подобными платформами (Linux, macOS, FreeBSD) для повторного использования этой общей реализации.
namespace unix_ {

/// \~english @brief Concrete Unix implementation of `IProcess` and `io::implementation::CStreamPort`.
///     Manages a child process, its I/O streams (stdin, stdout, stderr), and its state on Unix-like systems.
///     It typically uses `fork()` to create a child process, `exec()` family functions to load the new program,
///     and pipes to connect to the child's standard I/O streams. Process state is monitored using `waitpid()`.
///     This class is also synchronized using `threading::Synchronized<std::mutex>`.
/// \~russian @brief Конкретная Unix-реализация `IProcess` и `io::implementation::CStreamPort`.
///     Управляет дочерним процессом, его потоками ввода-вывода (stdin, stdout, stderr) и его состоянием
///     в Unix-подобных системах. Обычно использует `fork()` для создания дочернего процесса,
///     семейство функций `exec()` для загрузки новой программы и каналы (pipes) для подключения
///     к стандартным потокам ввода-вывода дочернего процесса. Состояние процесса отслеживается с помощью `waitpid()`.
///     Этот класс также синхронизирован с использованием `threading::Synchronized<std::mutex>`.
class CProcessStream:
    public IProcess,
    public io::implementation::CStreamPort,
    public threading::Synchronized<std::mutex>
{
public:
    /// \~english @brief Defines standard implementation helpers for `CProcessStream`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CProcessStream`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CProcessStream)
    /// \~english @brief Constructs a process stream for a given application and string of arguments.
    /// \~russian @brief Конструирует поток процесса для данного приложения и строки аргументов.
    /// \~english @param app The path to the executable application.
    /// \~russian @param app Путь к исполняемому приложению.
    /// \~english @param args A single string containing all arguments to be passed to the application. Arguments are typically space-separated. Defaults to an empty string if no arguments.
    /// \~russian @param args Одна строка, содержащая все аргументы для передачи приложению. Аргументы обычно разделены пробелами. По умолчанию пустая строка, если аргументы отсутствуют.
    CProcessStream(
        std::string const &app,
        std::string const &args = {});
    /// \~english @brief Constructs a process stream for a given application and a vector of arguments.
    /// \~russian @brief Конструирует поток процесса для данного приложения и вектора аргументов.
    /// \~english @param app The path to the executable application.
    /// \~russian @param app Путь к исполняемому приложению.
    /// \~english @param args A vector of strings, where each string is a separate argument to the application.
    /// \~russian @param args Вектор строк, где каждая строка является отдельным аргументом для приложения.
    CProcessStream(
        std::string const &app,
        std::vector<std::string> const &args);

    /// \~english @brief Initializes the process. This typically involves forking, setting up pipes for I/O, and executing the application in the child process.
    /// \~russian @brief Инициализирует процесс. Обычно включает `fork`, настройку каналов (pipes) для ввода-вывода и выполнение приложения в дочернем процессе.
    /// \~english @exception std::runtime_error if process creation or execution fails (e.g., fork, exec, pipe errors).
    /// \~russian @exception std::runtime_error если создание или выполнение процесса завершается неудачно (например, ошибки fork, exec, pipe).
    void initialize()   override;
    /// \~english @brief Finalizes the process. This typically involves waiting for the child process to terminate (if still running), closing pipes, and cleaning up resources.
    /// \~russian @brief Финализирует процесс. Обычно включает ожидание завершения дочернего процесса (если он еще выполняется), закрытие каналов (pipes) и освобождение ресурсов.
    /// \~english @exception std::runtime_error if errors occur during finalization, though often designed to be no-throw if possible.
    /// \~russian @exception std::runtime_error если возникают ошибки во время финализации, хотя часто проектируется как no-throw, если это возможно.
    void finalize()     override;

    /// \~english @brief Retrieves the current state of the process by querying the OS (e.g., using `waitpid` with `WNOHANG`).
    /// \~russian @brief Получает текущее состояние процесса путем запроса к ОС (например, с использованием `waitpid` с `WNOHANG`).
    /// \~english @return An `IProcess::TState` object representing the current condition and exit code (if terminated) of the process.
    /// \~russian @return Объект `IProcess::TState`, представляющий текущее состояние и код завершения (если завершен) процесса.
    TState getState()   override;
    void   sendSignal(TSignal const &signal) override;

private:
    /// \~english @brief Internal structure to hold detailed status information about the child process, typically populated from `waitpid()`.
    /// \~russian @brief Внутренняя структура для хранения подробной информации о состоянии дочернего процесса, обычно заполняемая из `waitpid()`.
    struct TStateInternal {
        /// \~english @brief True if the child process has exited normally.
        /// \~russian @brief True, если дочерний процесс завершился нормально.
        bool    is_exited;
        /// \~english @brief If `is_exited` is true, this holds the exit status code of the child process.
        /// \~russian @brief Если `is_exited` истинно, здесь хранится код статуса завершения дочернего процесса.
        uint8_t exit_status;
        /// \~english @brief True if the child process was terminated by a signal.
        /// \~russian @brief True, если дочерний процесс был завершен сигналом.
        bool    is_signaled;
        /// \~english @brief If `is_signaled` is true, this holds the number of the signal that terminated the child process.
        /// \~russian @brief Если `is_signaled` истинно, здесь хранится номер сигнала, который завершил дочерний процесс.
        uint8_t terminate_signal;
        /// \~english @brief True if a core dump was created for the terminated child process.
        /// \~russian @brief True, если для завершенного дочернего процесса был создан дамп ядра.
        bool    is_coredumped;
        /// \~english @brief True if the child process is currently stopped by a signal.
        /// \~russian @brief True, если дочерний процесс в данный момент остановлен сигналом.
        bool    is_stopped;
        /// \~english @brief If `is_stopped` is true, this holds the number of the signal that stopped the child process.
        /// \~russian @brief Если `is_stopped` истинно, здесь хранится номер сигнала, который остановил дочерний процесс.
        uint8_t stop_signal;
        /// \~english @brief True if a previously stopped child process has been continued.
        /// \~russian @brief True, если ранее остановленный дочерний процесс был продолжен.
        bool    is_continued;
    };

    /// \~english @brief Stores the detailed internal state of the child process.
    /// \~russian @brief Хранит подробное внутреннее состояние дочернего процесса.
    TStateInternal              m_state_internal;
    /// \~english @brief Path to the application executable.
    /// \~russian @brief Путь к исполняемому файлу приложения.
    std::string                 m_app;
    /// \~english @brief Vector of arguments to be passed to the application.
    /// \~russian @brief Вектор аргументов для передачи приложению.
    std::vector<std::string>    m_args;
    /// \~english @brief The full command line (application + arguments) used for logging/debugging.
    /// \~russian @brief Полная командная строка (приложение + аргументы), используемая для логирования/отладки.
    std::string                 m_command_line;
    /// \~english @brief Process ID (PID) of the child process. Atomic for thread-safe access if needed, though primary synchronization is via mutex.
    /// \~russian @brief Идентификатор процесса (PID) дочернего процесса. Атомарный для потокобезопасного доступа при необходимости, хотя основная синхронизация осуществляется через мьютекс.
    std::atomic<pid_t>          m_pid;

    /// \~english @brief Shared pointer to store the exit code of the process once it has terminated. `nullptr` if not yet terminated or code not available.
    /// \~russian @brief Разделяемый указатель для хранения кода завершения процесса после его завершения. `nullptr`, если еще не завершен или код недоступен.
    std::shared_ptr<int>        m_exit_code;
};


} // unix_
} // platform
} // implementation
} // system
} // iridium


#endif  // UNIX_PLATFORM


#endif // HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F
