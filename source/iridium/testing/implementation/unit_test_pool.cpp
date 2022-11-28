#include "unit_test_pool.h"
#include "iridium/system/implementation/process.h"
#include "iridium/io/implementation/event_provider.h"



#include "iridium/logging/logger.h"
using std::string;
using std::chrono::seconds;
using std::chrono::system_clock;
using iridium::convertion::convert;
using iridium::io::Buffer;
using iridium::system::implementation::CProcessStream;


seconds DEFAULT_PROCESS_OUTPUT_TIMEOUT(10);


namespace iridium {
namespace testing {
namespace implementation {


CUnitTestHandler::CUnitTestHandler(string const &app_path)
:
    m_app_path(app_path)
{}


CUnitTestHandler::TOutputItems CUnitTestHandler::handle(TInputItems const &test_command_line) {
    TOutputItems results;

    TResult result {
        .path = ""
    };
    for (auto const &test_args: test_command_line) {
        auto process = CProcessStream::create(m_app_path, test_args);
        process->initialize();
        auto timeout = system_clock::now() + DEFAULT_PROCESS_OUTPUT_TIMEOUT;
        while (true) {
            for (auto const &event: m_event_provider->waitEvents()) {
                if (event->getStream() == process &&
                    event->getType()   == io::IEvent::TType::READ)
                {
                    for (;;) {
                        auto buffer = process->read();
                        if (buffer)
                            result.output += convert<string>(*buffer);
                    };
                }
            }
            if (system_clock::now() > timeout) {
                LOGT << "test timeout !!!";
                result.output += "test timeout " + convert<string>(DEFAULT_PROCESS_OUTPUT_TIMEOUT);
                break;
            }
        }
        process->finalize();
    }
    results.push_back(result);

    return results;
}


} // implementation
} // testing
} // iridium
