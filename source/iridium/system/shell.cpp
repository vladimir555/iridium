#include "shell.h"

#include "iridium/system/implementation/process.h"
#include "iridium/io/implementation/multiplexer.h"
#include "iridium/logging/logger.h"
#include "iridium/items.h"

#include "process.h"


using std::string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

using iridium::system::implementation::CProcessStream;
using iridium::io::implementation::CMultiplexer;
using iridium::convertion::convert;


namespace iridium::system {


Shell::TTimeDuration const Shell::DEFAULT_TIMEOUT = std::chrono::minutes(10);


Shell::Shell()
:
    m_multiplexer(CMultiplexer::create())
{}


void Shell::initialize() {
    try {
        m_multiplexer->initialize();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("shell initialization error: ") + e.what());
    }
}


void Shell::finalize() {
    try {
        m_multiplexer->finalize();
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("shell finalization error: ") + e.what());
    }
}


Shell::TResult Shell::run(std::string const &command_line, TTimeDuration const &timeout) {
    Shell::TResult  result  = {};
    // todo: refactor platform::makeShellCommand
#ifdef WINDOWS_PLATFORM
    auto process = CProcessStream::create("powershell", std::vector<std::string>{ "-command", command_line } );
#else
    auto process = CProcessStream::create("sh", std::vector<std::string>{ "-c", command_line } );
#endif // WINDOWS_PLATFORM

    try {
        process->initialize();
        m_multiplexer->subscribe(process);

        auto now        = std::chrono::system_clock::now();
        auto stop_time  = now + timeout;
        auto state      = process->getState();

        for (;;) {
            for (auto const &event: m_multiplexer->waitEvents()) {
                // LOGT << event;

                using io::Event;

                if (event->stream == process &&
                    checkOneOf(
                        event->operation,
                        Event::TOperation::OPEN,
                        Event::TOperation::READ,
                        Event::TOperation::TIMEOUT))
                {
                    for (;;) {
                        auto buffer = process->read();
                        // LOGT << "buffer: '" << buffer << "'";

                        now = std::chrono::system_clock::now();

                        if (buffer && !buffer->empty() && now < stop_time) {
                            result.output += convert<string>(buffer);
                        } else {
                            break; // ----->
                        }
                    }
                }
            }

            state = process->getState();

            if (now > stop_time)
                state.condition  = IProcess::TState::TCondition::TIMEOUT;

            if (state.condition != IProcess::TState::TCondition::RUNNING)
                break; // --->
        }

        process->finalize();
        m_multiplexer->unsubscribe(process);

        if (state.condition == IProcess::TState::TCondition::DONE) {
            state = process->getState();
            result.code = state.exit_code ? *state.exit_code : -1;
            return result; // ----->
        } else {
            if (state.condition == IProcess::TState::TCondition::TIMEOUT)
                throw std::runtime_error("timeout " + convert<string>(timeout));
            else
                throw std::runtime_error("invalid process condition " + convert<string>(state.condition));
        }
    } catch (std::exception const &e) {
         m_multiplexer->unsubscribe(process);
        throw std::runtime_error("shell running command '" + command_line + "' error: " + e.what());
    }
}


} // namespace iridium::system
