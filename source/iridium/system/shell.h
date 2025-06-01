// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
#define HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2


#include <string>

#include "iridium/io/multiplexer.h"

#include "process.h"


namespace iridium {
namespace system {

/// \~english @brief Represents a shell command to be executed as an external process.
///     This class provides functionality to construct a command from an application path and arguments,
///     run the external process, manage its execution time with a configurable timeout,
///     and retrieve its exit status and standard output. It uses an underlying `IProcess`
///     and an `IMultiplexer` for event-driven I/O and timeout management.
/// \~russian @brief Представляет команду оболочки для выполнения в виде внешнего процесса.
///     Этот класс предоставляет функциональность для формирования команды из пути к приложению и аргументов,
///     запуска внешнего процесса, управления временем его выполнения с настраиваемым тайм-аутом,
///     а также получения его статуса завершения и стандартного вывода. Использует базовый `IProcess`
///     и `IMultiplexer` для событийно-ориентированного ввода-вывода и управления тайм-аутом.
class Command {
public:
    /// \~english @brief Type alias for specifying time durations, typically used for command execution timeouts. Based on `std::chrono::seconds`.
    /// \~russian @brief Псевдоним типа для указания продолжительности времени, обычно используемый для тайм-аутов выполнения команд. Основан на `std::chrono::seconds`.
    typedef std::chrono::seconds TTimeDuration;

    /// \~english @brief Default timeout duration for command execution if not otherwise specified.
    /// \~russian @brief Продолжительность тайм-аута по умолчанию для выполнения команды, если не указано иное.
    static TTimeDuration const DEFAULT_TIMEOUT;

    /// \~english @brief Constructs a command with application path, arguments, and an optional timeout.
    /// \~russian @brief Конструирует команду с путем к приложению, аргументами и опциональным тайм-аутом.
    /// \~english @param application_path The full path to the executable application.
    /// \~russian @param application_path Полный путь к исполняемому приложению.
    /// \~english @param arguments A string containing all arguments to be passed to the application.
    /// \~russian @param arguments Строка, содержащая все аргументы для передачи приложению.
    /// \~english @param timeout The maximum duration to wait for the command to complete. Defaults to `DEFAULT_TIMEOUT`.
    /// \~russian @param timeout Максимальная продолжительность ожидания завершения команды. По умолчанию `DEFAULT_TIMEOUT`.
    Command(
        std::string     const &application_path,
        std::string     const &arguments,
        TTimeDuration   const &timeout = DEFAULT_TIMEOUT
    );
    /// \~english @brief Constructs a command with application path and an optional timeout (no arguments).
    /// \~russian @brief Конструирует команду с путем к приложению и опциональным тайм-аутом (без аргументов).
    /// \~english @param application_path The full path to the executable application.
    /// \~russian @param application_path Полный путь к исполняемому приложению.
    /// \~english @param timeout The maximum duration to wait for the command to complete. Defaults to `DEFAULT_TIMEOUT`.
    /// \~russian @param timeout Максимальная продолжительность ожидания завершения команды. По умолчанию `DEFAULT_TIMEOUT`.
    Command(
        std::string     const &application_path,
        TTimeDuration   const &timeout = DEFAULT_TIMEOUT
    );
    /// \~english @brief Destructor for the Command object.
    ///     Ensures that the associated process is properly cleaned up if it was running.
    /// \~russian @brief Деструктор для объекта Command.
    ///     Гарантирует, что связанный процесс будет корректно очищен, если он был запущен.
    ~Command();

    /// \~english @brief Custom exception class for errors occurring during command execution.
    ///     This exception may be thrown by the `run()` methods if the command fails to start,
    ///     crashes, is interrupted, or times out. It provides access to the process state at the time of failure.
    /// \~russian @brief Пользовательский класс исключений для ошибок, возникающих во время выполнения команды.
    ///     Это исключение может быть выброшено методами `run()`, если команда не запускается,
    ///     падает, прерывается или истекает тайм-аут. Предоставляет доступ к состоянию процесса на момент сбоя.
    class Exception: public std::exception {
    public:
        /// \~english @brief Constructs a Command::Exception.
        /// \~russian @brief Конструирует Command::Exception.
        /// \~english @param what A string describing the error.
        /// \~russian @param what Строка, описывающая ошибку.
        /// \~english @param state The state of the process (`IProcess::TState`) when the exception occurred.
        /// \~russian @param state Состояние процесса (`IProcess::TState`) на момент возникновения исключения.
        Exception(std::string const &what, IProcess::TState const &state);
        /// \~english @brief Returns a C-style string describing the general cause of the current error. Overrides `std::exception::what()`.
        /// \~russian @brief Возвращает строку в стиле C, описывающую общую причину текущей ошибки. Переопределяет `std::exception::what()`.
        /// \~english @return Pointer to a C-style string with explanatory information.
        /// \~russian @return Указатель на строку в стиле C с пояснительной информацией.
        char const *what() const noexcept override;
        /// \~english @brief Gets the state of the process at the time the exception occurred.
        /// \~russian @brief Получает состояние процесса на момент возникновения исключения.
        /// \~english @return An `IProcess::TState` object representing the process's condition and exit code (if available).
        /// \~russian @return Объект `IProcess::TState`, представляющий состояние процесса и код завершения (если доступен).
        IProcess::TState getState() const;
    private:
        /// \~english @brief Stores the error message.
        /// \~russian @brief Хранит сообщение об ошибке.
        std::string         m_what;
        /// \~english @brief Stores the process state at the time of error.
        /// \~russian @brief Хранит состояние процесса на момент ошибки.
        IProcess::TState    m_state;
    };

    /// \~english @brief Runs the configured command and waits for its completion or timeout.
    ///     Standard output and standard error from the command are not captured by this overload.
    /// \~russian @brief Запускает сконфигурированную команду и ожидает ее завершения или истечения тайм-аута.
    ///     Стандартный вывод и стандартная ошибка команды не захватываются этой перегрузкой.
    /// \~english @return The exit code of the command.
    /// \~russian @return Код завершения команды.
    /// \~english @exception Command::Exception if the command execution fails (e.g., timeout, crash, interruption).
    /// \~russian @exception Command::Exception если выполнение команды завершается неудачно (например, тайм-аут, сбой, прерывание).
    int run();
    /// \~english @brief Runs the configured command, waits for its completion or timeout, and captures its standard output.
    /// \~russian @brief Запускает сконфигурированную команду, ожидает ее завершения или истечения тайм-аута и захватывает ее стандартный вывод.
    /// \~english @param[out] output A string reference to store the standard output of the command.
    /// \~russian @param[out] output Ссылка на строку для сохранения стандартного вывода команды.
    /// \~english @return The exit code of the command.
    /// \~russian @return Код завершения команды.
    /// \~english @exception Command::Exception if the command execution fails (e.g., timeout, crash, interruption).
    /// \~russian @exception Command::Exception если выполнение команды завершается неудачно (например, тайм-аут, сбой, прерывание).
    int run(std::string &output);

private:
    /// \~english @brief Shared pointer to an event multiplexer, used for managing I/O events and timeouts for the process.
    /// \~russian @brief Разделяемый указатель на мультиплексор событий, используемый для управления событиями ввода-вывода и тайм-аутами для процесса.
    io::IMultiplexer::TSharedPtr    m_event_provider;
    /// \~english @brief Shared pointer to the underlying process interface representing the external command.
    /// \~russian @brief Разделяемый указатель на базовый интерфейс процесса, представляющий внешнюю команду.
    IProcess::TSharedPtr            m_process;
    /// \~english @brief The timeout duration for the command execution.
    /// \~russian @brief Продолжительность тайм-аута для выполнения команды.
    TTimeDuration                   m_timeout;
    /// \~english @brief The full command line string (application path + arguments) to be executed.
    /// \~russian @brief Полная строка команды (путь к приложению + аргументы) для выполнения.
    std::string                     m_command_line;
};


} // system
} // iridium


#endif // HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
