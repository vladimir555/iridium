#ifndef HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
#define HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2


#include <string>

#include "iridium/io/multiplexer.h"

#include "process.h"


namespace iridium {
namespace system {


class Command {
public:
    typedef std::chrono::seconds TTimeDuration;

    static TTimeDuration const DEFAULT_TIMEOUT;

    Command(
        std::string     const &application_path,
        std::string     const &arguments,
        TTimeDuration   const &timeout = DEFAULT_TIMEOUT
    );
    Command(
        std::string     const &application_path,
        TTimeDuration   const &timeout = DEFAULT_TIMEOUT
    );
    ~Command();

    class Exception: public std::exception {
    public:
        Exception(std::string const &what, IProcess::TState const &state);
        char const *what() const _NOEXCEPT override;
        IProcess::TState getState() const;
    private:
        std::string         m_what;
        IProcess::TState    m_state;
    };

    int run();
    int run(std::string &output);

private:
    io::IMultiplexer::TSharedPtr  m_event_provider;
    IProcess::TSharedPtr            m_process;
    TTimeDuration                   m_timeout;
    std::string                     m_command_line;
};


} // system
} // iridium


#endif // HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
