#ifndef HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
#define HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/system/process.h"
#include "iridium/io/implementation/stream_port.h"
#include "iridium/io/uri.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace iridium {
namespace system {
namespace implementation {
namespace platform {


class CProcessStream: public virtual IProcess {
public:
    DEFINE_IMPLEMENTATION(CProcessStream)
    CProcessStream(
        std::string const &app,
        std::string const &args);
    CProcessStream(
        std::string const &app,
        std::vector<std::string> const &args);
    
    void initialize()   override;
    void finalize()     override;

    std::list<uintptr_t>    getHandles() const override;
    io::URI::TSharedPtr     getURI() const override;
    io::Buffer::TSharedPtr  read(size_t const &size) override;
    size_t                  write(io::Buffer::TSharedPtr const &buffer) override;
    
    TState getState()   override;
private:
    std::string         m_app;
    std::string         m_args;
    std::string         m_command_line;
    std::string         m_pipe_name;
    //HANDLE              m_file_stdin_reader;
    HANDLE              m_file_stdout_writer;
    HANDLE              m_pipe_stdout_reader;
    //HANDLE              m_pipe_stdout_writer;
    PROCESS_INFORMATION m_process;
    OVERLAPPED mutable  m_overlapped;
    io::URI::TSharedPtr m_uri;

    static std::atomic<uint64_t> m_process_counter;
};


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
