#include "shell.h"

#include "iridium/system/implementation/process.h"
#include "iridium/io/implementation/multiplexer.h"
#include "iridium/logging/logger.h"


using std::string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

using iridium::system::implementation::CProcessStream;
//using iridium::io::IListener;
using iridium::io::implementation::CMultiplexer;
using iridium::convertion::convert;
using iridium::convertion::convertPtr;


namespace iridium {
namespace system {


Command::TTimeDuration const Command::DEFAULT_TIMEOUT = std::chrono::minutes(10);


Command::Exception::Exception(std::string const &what, IProcess::TState const &state)
:
    m_what  (what),
    m_state (state)
{}


char const *Command::Exception::what() const noexcept {
    return m_what.c_str();
}


IProcess::TState Command::Exception::getState() const {
    return m_state;
}


Command::Command(
    std::string     const &application_path,
    std::string     const &arguments,
    TTimeDuration   const &timeout)
:
    m_event_provider(CMultiplexer::create()),
    m_process       (CProcessStream::create(application_path, arguments)),
    m_timeout       (timeout),
    m_command_line  (application_path + " " + arguments)
{
    m_event_provider->initialize();
}


Command::Command(
    std::string     const &application_path,
    TTimeDuration   const &timeout)
:
    Command(application_path, "", timeout)
{}


Command::~Command() {
    m_event_provider->finalize();
}


int Command::run(std::string &output) {
    m_process->initialize();
    m_event_provider->subscribe(m_process);

    auto now        = std::chrono::system_clock::now();
    auto stop_time  = now + m_timeout;
    auto state      = m_process->getState();

    for (;;) {
        for (auto const &event: m_event_provider->waitEvents()) {
//            LOGT << event->getStream()->getID() << " " << event->getType();

            if (event->stream == m_process && event->operation == io::Event::TOperation::WRITE) {
                for (;;) {
                    auto buffer = m_process->read();

                    LOGT << "buffer: " << *buffer;// << " size: " << buffer->size();

                    now = std::chrono::system_clock::now();

                    if (buffer && !buffer->empty() && now < stop_time)
                        output += convertPtr(buffer);
                    else
                        break; // ----->

                    if (buffer->size() < io::DEFAULT_BUFFER_SIZE)
                        break; // ----->
                }
            }
        }

        state = m_process->getState();

        if (now > stop_time)
            state.condition  = IProcess::TState::TCondition::TIMEOUT;

        if (state.condition != IProcess::TState::TCondition::RUNNING)
            break; // --->
    }

    m_process->finalize();
    m_event_provider->unsubscribe(m_process);

    if (state.condition == IProcess::TState::TCondition::DONE) {
        auto state = m_process->getState();
        return state.exit_code ? *state.exit_code : -1; // ----->
    } else {
        string error = "execution command '" + m_command_line + "' error: ";
        if (state.condition == IProcess::TState::TCondition::TIMEOUT)
            error += "timeout " + convert<string>(m_timeout);
        else
            error += "wrong process condition " + convert<string>(state.condition);

        throw Exception(error, state);
    }
}


} // system
} // iridium
