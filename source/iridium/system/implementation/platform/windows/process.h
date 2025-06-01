#ifndef HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
#define HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM

/// \~english @file
/// @brief Windows-specific platform implementation for process management and I/O redirection.
///     This header defines `CProcessStream`, which is the concrete class for creating and managing
///     external processes on the Windows operating system. It utilizes Windows API functions
///     like `CreateProcess` for process creation, and named pipes or anonymous pipes for
///     redirecting the standard I/O streams (stdin, stdout, stderr) of the child process.
/// \~russian @file
/// @brief Платформо-специфичная реализация для Windows для управления процессами и перенаправления ввода-вывода.
///     Этот заголовочный файл определяет `CProcessStream`, который является конкретным классом для создания
///     и управления внешними процессами в операционной системе Windows. Он использует функции Windows API,
///     такие как `CreateProcess` для создания процесса, и именованные или анонимные каналы для
///     перенаправления стандартных потоков ввода-вывода (stdin, stdout, stderr) дочернего процесса.

#include "iridium/system/process.h"
#include "iridium/io/implementation/stream_port.h" // Likely CStreamPort not directly inherited but IProcess implies IStreamPort
#include "iridium/io/uri.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace iridium {
namespace system {
namespace implementation {
/// \~english @brief Contains platform-specific implementations for system components, tailored for the Windows environment.
/// \~russian @brief Содержит платформо-специфичные реализации системных компонентов, адаптированные для среды Windows.
namespace platform {

/// \~english @brief Concrete Windows implementation of the `IProcess` interface.
///     This class manages a child process, its I/O streams (stdin, stdout, stderr), and state
///     using Windows API calls such as `CreateProcess` for starting the process,
///     and Windows pipes (anonymous or named) for I/O redirection. It also handles
///     process state monitoring and finalization.
/// \~russian @brief Конкретная реализация интерфейса `IProcess` для Windows.
///     Этот класс управляет дочерним процессом, его потоками ввода-вывода (stdin, stdout, stderr) и состоянием,
///     используя вызовы Windows API, такие как `CreateProcess` для запуска процесса,
///     и каналы Windows (анонимные или именованные) для перенаправления ввода-вывода.
///     Он также обрабатывает мониторинг состояния процесса и его завершение.
class CProcessStream: public virtual IProcess { // Implements IProcess, which inherits IStreamPort
public:
    /// \~english @brief Defines standard implementation helpers for `CProcessStream`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CProcessStream`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CProcessStream)
    /// \~english @brief Constructs a process for a given application and a single string of arguments.
    /// \~russian @brief Конструирует процесс для данного приложения и одной строки аргументов.
    /// \~english @param app The path to the executable application.
    /// \~russian @param app Путь к исполняемому приложению.
    /// \~english @param args A single string containing all arguments to be passed to the application.
    /// \~russian @param args Одна строка, содержащая все аргументы для передачи приложению.
    CProcessStream(
        std::string const &app,
        std::string const &args);
    /// \~english @brief Constructs a process for a given application and a vector of arguments.
    /// \~russian @brief Конструирует процесс для данного приложения и вектора аргументов.
    /// \~english @param app The path to the executable application.
    /// \~russian @param app Путь к исполняемому приложению.
    /// \~english @param args A vector of strings, where each string is a separate argument. These will be combined into a command line.
    /// \~russian @param args Вектор строк, где каждая строка является отдельным аргументом. Они будут объединены в командную строку.
    CProcessStream(
        std::string const &app,
        std::vector<std::string> const &args);
    
    /// \~english @brief Initializes the process. On Windows, this involves setting up pipes for stdin/stdout/stderr, and calling `CreateProcess`.
    /// \~russian @brief Инициализирует процесс. В Windows это включает настройку каналов для stdin/stdout/stderr и вызов `CreateProcess`.
    /// \~english @exception std::runtime_error if process creation or pipe setup fails.
    /// \~russian @exception std::runtime_error если создание процесса или настройка каналов завершается неудачно.
    void initialize()   override;
    /// \~english @brief Finalizes the process. Ensures the process has terminated, closes handles to the process, threads, and pipes.
    /// \~russian @brief Финализирует процесс. Гарантирует завершение процесса, закрывает дескрипторы процесса, потоков и каналов.
    /// \~english @exception std::runtime_error if errors occur during finalization.
    /// \~russian @exception std::runtime_error если возникают ошибки во время финализации.
    void finalize()     override;

    /// \~english @brief Gets the native handles associated with this stream port (e.g., pipe handles for I/O).
    /// \~russian @brief Получает нативные дескрипторы, связанные с этим потоковым портом (например, дескрипторы каналов для ввода-вывода).
    /// \~english @return A list of `uintptr_t` representing the native OS handles. For Windows, these are `HANDLE` types cast to `uintptr_t`.
    /// \~russian @return Список `uintptr_t`, представляющих нативные дескрипторы ОС. Для Windows это типы `HANDLE`, приведенные к `uintptr_t`.
    std::list<uintptr_t>    getHandles() const override;
    /// \~english @brief Gets the URI associated with this stream port. For a process, this might be a unique identifier or the command line.
    /// \~russian @brief Получает URI, связанный с этим потоковым портом. Для процесса это может быть уникальный идентификатор или командная строка.
    /// \~english @return A shared pointer to an `iridium::io::URI` object.
    /// \~russian @return Разделяемый указатель на объект `iridium::io::URI`.
    io::URI::TSharedPtr     getURI() const override;
    /// \~english @brief Reads data from the process's standard output stream.
    /// \~russian @brief Читает данные из стандартного потока вывода процесса.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A shared pointer to an `iridium::io::Buffer` containing the data read. Returns `nullptr` or an empty buffer on EOF or error.
    /// \~russian @return Разделяемый указатель на `iridium::io::Buffer`, содержащий прочитанные данные. Возвращает `nullptr` или пустой буфер при EOF или ошибке.
    /// \~english @exception std::runtime_error if a read error occurs.
    /// \~russian @exception std::runtime_error если происходит ошибка чтения.
    io::Buffer::TSharedPtr  read(size_t const &size) override;
    /// \~english @brief Writes data to the process's standard input stream.
    /// \~russian @brief Записывает данные в стандартный поток ввода процесса.
    /// \~english @param buffer A shared pointer to an `iridium::io::Buffer` containing the data to write.
    /// \~russian @param buffer Разделяемый указатель на `iridium::io::Buffer`, содержащий данные для записи.
    /// \~english @return The number of bytes successfully written.
    /// \~russian @return Количество успешно записанных байт.
    /// \~english @exception std::runtime_error if a write error occurs.
    /// \~russian @exception std::runtime_error если происходит ошибка записи.
    size_t                  write(io::Buffer::TSharedPtr const &buffer) override;
    
    /// \~english @brief Retrieves the current state of the process (e.g., running, done, crashed) and its exit code if terminated.
    /// \~russian @brief Получает текущее состояние процесса (например, выполняется, завершен, аварийно завершен) и его код завершения, если он терминирован.
    /// \~english @return An `IProcess::TState` object.
    /// \~russian @return Объект `IProcess::TState`.
    TState getState()   override;
private:
    /// \~english @brief Path to the application executable.
    /// \~russian @brief Путь к исполняемому файлу приложения.
    std::string                     m_app;
    /// \~english @brief String of arguments to be passed to the application.
    /// \~russian @brief Строка аргументов для передачи приложению.
    std::string                     m_args;
    /// \~english @brief The full command line (application + arguments) to be executed.
    /// \~russian @brief Полная командная строка (приложение + аргументы) для выполнения.
    std::string                     m_command_line;
    /// \~english @brief Name for the named pipe used for stdout redirection. Generated to be unique.
    /// \~russian @brief Имя для именованного канала, используемого для перенаправления stdout. Генерируется уникальным.
    std::string                     m_pipe_name;
    /// \~english @brief Windows HANDLE for the write end of the process's standard output pipe. This end is used by the child process.
    /// \~russian @brief Дескриптор Windows для записывающего конца канала стандартного вывода процесса. Этот конец используется дочерним процессом.
    HANDLE                          m_file_stdout_writer;
    /// \~english @brief Windows HANDLE for the read end of the process's standard output pipe. This end is used by the parent process to read child's stdout.
    /// \~russian @brief Дескриптор Windows для читающего конца канала стандартного вывода процесса. Этот конец используется родительским процессом для чтения stdout дочернего процесса.
    HANDLE                          m_pipe_stdout_reader;
    //HANDLE              m_file_stdin_reader; // Potentially for stdin write end (parent)
    //HANDLE              m_pipe_stdout_writer; // Potentially for stdin read end (child)
    /// \~english @brief Windows structure containing information about the newly created process and its primary thread.
    /// \~russian @brief Структура Windows, содержащая информацию о вновь созданном процессе и его основном потоке.
    PROCESS_INFORMATION             m_process;
    /// \~english @brief Shared pointer to an `OVERLAPPED` structure, used for asynchronous I/O operations on pipes with `ReadFile`/`WriteFile`.
    /// \~russian @brief Разделяемый указатель на структуру `OVERLAPPED`, используемую для асинхронных операций ввода-вывода с каналами через `ReadFile`/`WriteFile`.
    std::shared_ptr<OVERLAPPED>     m_overlapped;
    /// \~english @brief URI representing this process stream, potentially for identification or resource management.
    /// \~russian @brief URI, представляющий этот поток процесса, возможно для идентификации или управления ресурсами.
    io::URI::TSharedPtr             m_uri;
    /// \~english @brief Windows structure used to specify security attributes for securable objects like pipes.
    /// \~russian @brief Структура Windows, используемая для указания атрибутов безопасности для защищаемых объектов, таких как каналы.
    SECURITY_ATTRIBUTES             m_security_attributes;
    /// \~english @brief Buffer used for storing data read from the process's stdout.
    /// \~russian @brief Буфер, используемый для хранения данных, прочитанных из stdout процесса.
    io::Buffer::TSharedPtr          m_buffer;

    /// \~english @brief Static atomic counter to help generate unique names for pipes or other resources per process instance.
    /// \~russian @brief Статический атомарный счетчик для генерации уникальных имен для каналов или других ресурсов для каждого экземпляра процесса.
    static std::atomic<uint64_t>    m_process_counter;
};


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
