#ifndef HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F
#define HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/implementation/stream_port.h"
#include "iridium/system/process.h"

#include <string>
#include <sched.h>


namespace iridium {
namespace system {
namespace implementation {
namespace platform {
namespace unix_ {


class CProcessStream: public IProcess, public io::implementation::CStreamPort {
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

    TState getState()   override;

private:
    struct TStateInternal {
        bool    is_exited;
        uint8_t exit_status;
        bool    is_signaled;
        uint8_t terminate_signal;
        bool    is_coredumped;
        bool    is_stopped;
        uint8_t stop_signal;
        bool    is_continued;
    };

    TStateInternal              m_state_internal;
    std::string                 m_app;
    std::vector<std::string>    m_args;
    std::string                 m_command_line;
    std::atomic<pid_t>          m_pid;
    int                         m_pipe_out;

    std::shared_ptr<int>        m_exit_code;
};


} // unix_
} // platform
} // implementation
} // system
} // iridium


#endif  // UNIX_PLATFORM


#endif // HEADER_PROCESS_5FCB4526_7069_4292_896C_A036DA173F9F
