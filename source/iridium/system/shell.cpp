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
using iridium::convertion::convertPtr;


namespace iridium {
namespace system {


Shell::TTimeDuration const Shell::DEFAULT_TIMEOUT = std::chrono::minutes(10);


Shell::Shell()
:
    m_multiplexer(CMultiplexer::create())
{}


void Shell::initialize() {
    m_multiplexer->initialize();
}


void Shell::finalize() {
    m_multiplexer->finalize();
}


Shell::TResult Shell::run(std::string const &command_line, TTimeDuration const &timeout) {
    Shell::TResult  result  = {};
    // todo: refactor platform::makeShellCommand
#ifdef WINDOWS_PLATFORM
    auto            process = CProcessStream::create("powershell", std::vector<std::string>{ "-command", command_line } );
#else
    auto            process = CProcessStream::create("sh", std::vector<std::string>{ "-c", command_line } );
#endif // WINDOWS_PLATFORM

    try {
        process->initialize();
        m_multiplexer->subscribe(process);

        auto now        = std::chrono::system_clock::now();
        auto stop_time  = now + timeout;
        auto state      = process->getState();

        for (;;) {
            for (auto const &event: m_multiplexer->waitEvents()) {
                // LOGT << event->stream->getURI() << " " << event->status;

                if (event->stream == process &&
                    checkOneOf(
                        event->operation,
                        io::Event::TOperation::READ,
                        io::Event::TOperation::EOF_,
                        io::Event::TOperation::TIMEOUT))
                {
                    for (;;) {
                        auto buffer = process->read();

                        // LOGT << "buffer: " << buffer;// << " size: " << buffer->size();

                        now = std::chrono::system_clock::now();

                        if (buffer && !buffer->empty() && now < stop_time) {
                            result.output += convertPtr(buffer);

                            if (buffer->size() < io::DEFAULT_BUFFER_SIZE)
                                break; // ----->
                        } else
                            break; // ----->
                    }
                }
            }

            state = process->getState();

            if (now > stop_time)
                state.condition  = IProcess::TState::TCondition::TIMEOUT;

            if (state.condition != IProcess::TState::TCondition::RUNNING)
                break; // --->
        }

        m_multiplexer->unsubscribe(process);
        process->finalize();

        if (state.condition == IProcess::TState::TCondition::DONE) {
            auto state = process->getState();
            result.code = state.exit_code ? *state.exit_code : -1;
            return result; // ----->
        } else {
            if (state.condition == IProcess::TState::TCondition::TIMEOUT)
                throw std::runtime_error("timeout " + convert<string>(timeout));
            else
                throw std::runtime_error("invalid process condition " + convert<string>(state.condition));
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("shell command '" + command_line + "' error: " + e.what());
    }
}


} // system
} // iridium
