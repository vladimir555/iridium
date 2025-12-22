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


#ifdef _MSC_VER
#pragma warning(disable : 4250)
#endif


class CProcessStream:
    public IProcess,
    public io::implementation::CStreamPort
{
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

    TState getState() override;
    void   sendSignal(TSignal const &signal) override;

private:
    static std::atomic<uint64_t>    m_process_counter;

    io::URI::TSharedPtr             m_uri;
    std::string                     m_app;
    std::string                     m_args;
    std::string                     m_command_line;
    PROCESS_INFORMATION             m_process;
    SECURITY_ATTRIBUTES             m_security_attributes;
    TState::TSharedPtr              m_finalized_state;
};


} // iridium::system::implementation::platform


#endif // WINDOWS_PLATFORM


#endif // HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
