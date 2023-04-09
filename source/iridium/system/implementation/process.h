// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0
#define HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0


#include "iridium/io/implementation/stream_port.h"
#include "iridium/system/process.h"

#include <string>
#include <sched.h>


namespace iridium {
namespace system {
namespace implementation {


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


} // implementation
} // system
} // iridium


#endif // HEADER_PROCESS_D2A41C18_6019_439D_AFE4_2EF3834E06D0
