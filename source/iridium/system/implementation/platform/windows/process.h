#ifndef HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
#define HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/system/process.h"
#include "iridium/io/implementation/stream_port.h"
#include "iridium/io/uri.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace iridium::system::implementation::platform {


/// \~english @brief A class for managing a process and its streams on Windows.
/// \~russian @brief Класс для управления процессом и его потоками в Windows.
class CProcessStream: public virtual IProcess {
public:
    DEFINE_IMPLEMENTATION(CProcessStream)
    /// \~english @brief Constructor with application path and arguments as a single string.
    /// \~russian @brief Конструктор с путем к приложению и аргументами в виде одной строки.
    CProcessStream(
        std::string const &app,
        std::string const &args);
    /// \~english @brief Constructor with application path and arguments as a vector of strings.
    /// \~russian @brief Конструктор с путем к приложению и аргументами в виде вектора строк.
    CProcessStream(
        std::string const &app,
        std::vector<std::string> const &args);

    void initialize()   override;
    /// \~english @brief Finalizes the process and its streams.
    /// \~russian @brief Финализирует процесс и его потоки.
    void finalize()     override;

    /// \~english @brief Gets the handles of the process streams.
    /// \~russian @brief Возвращает дескрипторы потоков процесса.
    std::list<uintptr_t>    getHandles() const override;
    /// \~english @brief Gets the URI of the process.
    /// \~russian @brief Возвращает URI процесса.
    io::URI::TSharedPtr     getURI() const override;
    /// \~english @brief Reads data from the process's output stream.
    /// \~russian @brief Читает данные из выходного потока процесса.
    io::Buffer::TSharedPtr  read(size_t const &size = io::DEFAULT_BUFFER_SIZE) override;
    /// \~english @brief Writes data to the process's input stream.
    /// \~russian @brief Записывает данные во входной поток процесса.
    size_t                  write(io::Buffer::TSharedPtr const &buffer) override;

    TState getState() override;
    /// \~english @brief Sends a signal to the process.
    /// \~russian @brief Отправляет сигнал процессу.
    void   sendSignal(TSignal const &signal) override;

private:
    std::string                     m_app;
    std::string                     m_args;
    std::string                     m_command_line;
    std::string                     m_pipe_name;
    HANDLE                          m_file_stdout_writer;
    HANDLE                          m_pipe_stdout_reader;
    //HANDLE              m_file_stdin_reader;
    //HANDLE              m_pipe_stdout_writer;
    PROCESS_INFORMATION             m_process;
    std::shared_ptr<OVERLAPPED>     m_overlapped;
    io::URI::TSharedPtr             m_uri;
    SECURITY_ATTRIBUTES             m_security_attributes;
    io::Buffer::TSharedPtr          m_buffer;

    static std::atomic<uint64_t>    m_process_counter;
};


} // iridium::system::implementation::platform


#endif // WINDOWS_PLATFORM


#endif // HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
