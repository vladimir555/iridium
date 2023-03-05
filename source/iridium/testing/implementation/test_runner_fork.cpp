#include "test_runner_fork.h"

#include "iridium/io/implementation/session_manager.h"
#include "iridium/io/implementation/pipe.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/system/implementation/process.h"
#include "iridium/threading/implementation/async_queue.h"

#include "iridium/parsing/implementation/parser_json.h"

#include "iridium/logging/logger.h"
#include "iridium/items.h"


using iridium::io::implementation::CSessionManager;
using iridium::io::implementation::CPipe;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::system::implementation::CProcessStream;
using iridium::system::IProcess;
using iridium::threading::implementation::CAsyncQueue;
using iridium::threading::IAsyncQueuePusher;
using iridium::convertion::convert;

using std::string;
using std::list;
using std::chrono::milliseconds;

using iridium::parsing::implementation::CJSONParser;


namespace iridium {
namespace testing {
namespace implementation {


std::chrono::seconds const CTestRunnerFork::DEFAULT_TIMEOUT(10);


CTestRunnerFork::CTestRunnerFork(
    std::string     const &app_path,
    milliseconds    const &timeout)
:
    m_app_path          (app_path),
    m_timeout           (timeout),
    m_session_manager   (CSessionManager::create())
{}


TTestResult CTestRunnerFork::run(INodeTest::TSharedPtr const &node_test) {
    TTestResult test_results;
    m_session_manager->initialize();

    list<string> paths;
    scan(node_test, "", paths);

    std::unordered_map<std::string, CTestProtocolHandler::TSharedPtr> map_path_handler;
    auto process_result_queue = CAsyncQueue<TProcessResult::TConstSharedPtr>::create();

    for (auto const &path: paths) {
//        LOGT << path;

        auto process = CProcessStream::create(m_app_path, "run --json-result " + path);
        auto handler = CTestProtocolHandler::create(process, path, process_result_queue);

        map_path_handler[path] = handler;
        m_session_manager->manage(process, handler);
//        LOGT << "process start: " << path;
    }

//    if (!m_session_manager->wait(std::chrono::seconds(50)))
//        LOGW << "tests timeout";

    auto parser = CJSONParser::create();

    int count_wait = paths.size();

    while (count_wait > 0) {
        auto results = process_result_queue->pop(m_timeout);
        if  (results.empty())
            break; // --->

        for (auto const &result: results) {
            count_wait--;
//            LOGT << "process stop:  " << result->path;
            if (/*result->state.condition == IProcess::TState::TCondition::DONE &&*/
                result->state.exit_code && *result->state.exit_code == 1)
            {
//                test.Error = "";
//                test_result.Test.add(test);
            } else {
                TTestResult::TTest test;
                test.Path   = result->path;
                test.Error  = convert<string>( result->state.condition);
                test.Output = convert<string>(*result->output);
                test_results.Test.add(test);
                continue; // <---
            }
//            test_result.Test.add(test);

            string output;
            try {
                output = convert<string>(*result->output);
                auto right  = output.find_last_not_of('\n');
                auto left   = output.find_last_of('\n', right);
                auto size   = convert<uint32_t>(output.substr(left + 1, right - left));
                // todo: bugfix parser
                auto node   = parser->parse(output.substr(left - size - 1, size))->getChild("test-result");
                TTestResult test_results_fork(node);
                for (auto const &test: test_results_fork.Test) {
                    test_results.Test.add(test);
                }
                output = output.substr(0, left - size);
                LOGI << "\n" << result->path << "\n" << output;
            } catch (...) {
                LOGF << "test process parsing output error:\n-----\n" << output << "\n-----";
            }

            map_path_handler.erase(result->path);

            if (map_path_handler.empty())
                break; // --->
        }
    }

    m_session_manager->finalize();

    if (!map_path_handler.empty()) {
        string msg = "\n\ntimeout:\n";

        for (auto const &path_handler: map_path_handler) {
            msg += path_handler.first + ":\n";
            if (path_handler.second->getBuffer())
                msg += convertion::convert<string>(*path_handler.second->getBuffer()) + "\n";
        }

        LOGE << "\n-----" << msg << "\n-----";
    }

    return test_results;
}


void CTestRunnerFork::scan(
    INodeTest::TSharedPtr const &node,
    string                const &path,
    list<std::string>           &paths)
{
    for (auto const &child: *node) {
        if (child->hasChilds() && !child->begin()->get()->hasChilds()) {
            paths.push_back(path + "/" + child->getName());
//            LOGT << path + "/" + child->getName();
        }
        scan(child, path + "/" + child->getName(), paths);
    }
}


CTestRunnerFork::CTestProtocolHandler::CTestProtocolHandler(
    IProcess::TSharedPtr const &process,
    string const &path,
    IAsyncQueuePusher<TProcessResult::TConstSharedPtr>::TSharedPtr const &process_result_queue)
:
    m_process               (process),
    m_path                  (path),
    m_process_result_queue  (process_result_queue)
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::IEvent::TSharedPtr const &event,
    io::IPipeManager::TSharedPtr const &pipe_manager)
{
    LOGT << "event: " << event->getType() << ", fd " << (event->getType() == io::IEvent::TType::OPEN ? 0 : event->getStream()->getID());

    try {
        bool is_parsed = false;
//        if (event->getType() == io::IEvent::TType::OPEN)
//            LOGT << "0 event: " << event->getType();
//        else
//            LOGT << "0 event: " << event->getType() << " " << event->getStream()->getID();
        // todo: optimize !!!
        try {
            if (m_state.exit_code && m_buffer_output && m_buffer_output->size() > 1) {
//                LOGT << "1 exit_code: " << m_state.exit_code;
                // todo: fix checking with size == size
                auto s = std::string(m_buffer_output->begin(), m_buffer_output->end());
                auto l = s.find_last_of('}');

                if (s.substr(l, 3) == "}\n\n") {
                    l += 3;
                    auto r = s.find_first_of('\n', l);
//                    LOGT << "2 size: '" << s.substr(l, r - l) << "', str size: " << s.size();
                    if (r != std::string::npos && convert<uint64_t>(s.substr(l, r - l)) > 0)
                        is_parsed = true;
                }
            }
        } catch (...) {

        }
    //    if (m_buffer_output)
    //        LOGT << "buffer:\n" << *m_buffer_output;

        if (event->getType() == io::IEvent::TType::OPEN) {
            pipe_manager->createPipe("process");
            pipe_manager->updateReader("process", std::dynamic_pointer_cast<io::IStreamReader>(event->getStream()));
            m_buffer_output = io::Buffer::create();
            m_stream_output = CStreamWriterBuffer::create(m_buffer_output);
            pipe_manager->updateWriter("process", m_stream_output);
        }

        m_state = m_process->getState();

    //    LOGT << __FUNCTION__
    //         << "\nevent: " << event->getStream()->getID()
    //         << " " << event->getType()
    ////         << "\nbuffer:\n" << *m_buffer_output
    //         << "\nstate: " << m_state.condition;

        if (is_parsed) {
            m_process_result_queue->push(
                TProcessResult::create(
                    TProcessResult {
                        .path   = m_path,
                        .state  = m_state,
                        .output = m_buffer_output
                    }
                )
            );
        }

        //threading::sleep(50);

        m_state = m_process->getState();

    //    std::cout << "THREAD END  : " << static_cast<uint64_t>(uintptr_t(this)) << " " <<
    //        convert<string>(std::this_thread::get_id()) << std::endl;
        return !is_parsed;
    } catch (std::exception const &e) {
        string what(e.what());
        // todo: refactor inserting
        if (!m_buffer_output)
            m_buffer_output = io::Buffer::create();
        m_buffer_output->push_back('\n');
        m_buffer_output->insert(m_buffer_output->begin(), what.begin(), what.end());
        m_process_result_queue->push(
            TProcessResult::create(
                TProcessResult {
                    .path   = m_path,
                    .state  = { IProcess::TState::TCondition::INTERRUPTED, 0 },
                    .output = m_buffer_output
                }
            )
        );
        return false;
    }
}


io::Buffer::TSharedPtr CTestRunnerFork::CTestProtocolHandler::getBuffer() const {
    return m_buffer_output;
}


IProcess::TState CTestRunnerFork::CTestProtocolHandler::getExitState() const {
    return m_state; // ----->
}


} // implementation
} // testing
} // iridium
