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


TResult CTestRunnerFork::run(INodeTest::TSharedPtr const &node_test) {
    TResult test_results;
    m_session_manager->initialize();

    list<string> paths;
    scan(node_test, "", paths);

    std::unordered_map<std::string, CTestProtocolHandler::TSharedPtr> map_path_handler;
    auto process_result_queue = CAsyncQueue<TProcessResult::TConstSharedPtr>::create();

    for (auto const &path: paths) {
//        LOGT << path;

        auto process = CProcessStream::create(m_app_path, "run --mode=raw --print-result=json " + path);
        auto handler = CTestProtocolHandler::create(process, path, process_result_queue);

        map_path_handler[path] = handler;
        m_session_manager->manage(process, handler);
    }

    auto parser = CJSONParser::create();

    int count_wait = paths.size();

    std::list<TProcessResult::TConstSharedPtr> interrupted;

    while (count_wait > 0) {
//        LOGT << "wait queue: " << count_wait;
        auto test_fork_handler_results = process_result_queue->pop(m_timeout);

        if  (test_fork_handler_results.empty())
            break; // --->

        for (auto const &result: test_fork_handler_results) {
            count_wait--;
//            LOGT << "count_wait: " << count_wait;

            if (result->node) {
//                LOGT << result->node;
                TResult test_results_fork(result->node);
                for (auto const &test: test_results_fork.Tests)
                    test_results.Tests.add(test);
            }
            map_path_handler.erase(result->path);
        }

        auto process_result = test_fork_handler_results.back();

        if (process_result->state.condition == IProcess::TState::TCondition::DONE)
            LOGI << process_result->path << ":\n"
                 << process_result->output;
        else {
            for (auto const &node: *node_test->slice(process_result->path).back()) {
//                LOGT << "SLICE: " << node;

                TResult::TTests test;
                test.Path   = process_result->path + "/" + node->getName();

                test.Error  = convert<string>(process_result->state.condition);
                test_results.Tests.add(test);
            }
            interrupted.push_back(process_result);
            LOGF << process_result->path << ":\n"
                 << process_result->state.condition;
        }
    }

//    LOGT << "session manager finalize ... ";
    m_session_manager->finalize();
//    LOGT << "session manager finalize OK";

    if (!interrupted.empty()) {
        LOGF << "\nINTERRUPTED:\n";
        for (auto const &process_result: interrupted)
            LOGF << process_result->state.condition << " "
                 << process_result->path << ":\n\n"
                 << process_result->output;
    }

    for (auto const &path_handler: map_path_handler) {
        TResult::TTests test;
//        for (auto const &node: node_test->)
        test.Path   = path_handler.first;
        test.Error  = convert<string>(path_handler.second->getExitState().condition);
        if (path_handler.second->getBuffer())
            test.Output = convert<string>(*path_handler.second->getBuffer());
        test_results.Tests.add(test);
    }

//    if (!map_path_handler.empty()) {
//        string msg = "\n\ntimeout:\n";
//        for (auto const &path_handler: map_path_handler) {
//            msg += path_handler.first + ":\n";
//            if (path_handler.second->getBuffer())
//                msg += convert<string>(*path_handler.second->getBuffer()) + "\n";
//        }
//        LOGE << "\n-----" << msg << "\n-----";
//    }

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
    m_process_result_queue  (process_result_queue),
    m_is_finished           (false),
    m_parser                (CJSONParser::create())
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::IEvent::TSharedPtr const &event,
    io::IPipeManager::TSharedPtr const &pipe_manager)
{
    if (m_is_finished)
        return false; // ----->

//    LOGT << "event: " <<  event->getType()
//         << ", fd "   << (event->getType() == io::IEvent::TType::OPEN ? 0 : event->getStream()->getID())
//         << ". process state: " << m_state.condition;

    m_state = m_process->getState();

    if (event->getType() == io::IEvent::TType::OPEN) {
        pipe_manager->createPipe("process");
        pipe_manager->updateReader("process", std::dynamic_pointer_cast<io::IStreamReader>(event->getStream()));
        m_buffer_output = io::Buffer::create();
        m_stream_output = CStreamWriterBuffer::create(m_buffer_output);
        pipe_manager->updateWriter("process", m_stream_output);
        return true; // ----->
    }

    try {
        if (m_state.condition != IProcess::TState::TCondition::RUNNING &&
            m_buffer_output             &&
            m_buffer_output->size() > 4 &&
            m_buffer_output->back() == '\n')
        {
            size_t right    = m_buffer_output->size() - 1;
            size_t left     = right;

            while (left > 0 && m_buffer_output->at(left - 1) != '\n')
                left--;

            auto size = convert<uint64_t>(string(m_buffer_output->begin() + left, m_buffer_output->begin() + right));


            if (m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '}'  &&
                m_buffer_output->at(left - size + 2) == '{')
            {
                right   = left + 1;
                left    = left - size + 2;
            } else
                return true; // ----->

//            try {
            string json(m_buffer_output->begin() + left, m_buffer_output->begin() + right);
            auto node = m_parser->parse(json);

            m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());

            m_process_result_queue->push(
                TProcessResult::create(
                    TProcessResult {
                        .path   = m_path,
                        .state  = m_state,
                        .output = m_buffer_output,
                        .node   = node
                    }
                )
            );

            m_is_finished = true;
//                LOGT << "PARSING DONE";
//            } catch (std::exception const &e) {
//                LOGT << e.what();
//            } catch (...) {

//            }
        }

//        LOGT << "stop handler, parsing done, m_is_finished: " << m_is_finished;
        return !m_is_finished;
//    } catch (std::exception const &e) {
    } catch (...) {
//        string what(e.what());
        // todo: refactor inserting
        if(!m_buffer_output)
            m_buffer_output = io::Buffer::create();
//        m_buffer_output->push_back('\n');
//        m_buffer_output->insert(m_buffer_output->begin(), what.begin(), what.end());
//        m_buffer_output->push_back('\n');
        m_process_result_queue->push(
            TProcessResult::create(
                TProcessResult {
                    .path   = m_path,
                    .state  = m_state,
                    .output = m_buffer_output
                }
            )
        );
        m_is_finished = true;
//        LOGT << "PARSING FAILED";
//        LOGT << "stop handler, parsing failed, m_is_finished: " << m_is_finished;
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
