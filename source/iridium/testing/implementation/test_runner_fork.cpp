#include "test_runner_fork.h"

#include "iridium/io/implementation/session_manager.h"
#include "iridium/io/implementation/pipe.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/system/implementation/process.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/parsing/implementation/parser_json.h"
#include "iridium/logging/logger.h"
#include "iridium/items.h"
#include "iridium/assert.h"


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
        auto process = CProcessStream::create(m_app_path, "run --mode=raw --print-result=json " + path);
        auto handler = CTestProtocolHandler::create(process, path, process_result_queue);

        map_path_handler[path] = handler;
        m_session_manager->manage(process, handler);
    }

    auto parser = CJSONParser::create();

    int paths_left = paths.size();

    std::list<TProcessResult::TConstSharedPtr> interrupted;

    while (paths_left > 0) {
//        LOGT << "paths left: " << paths_left;
        auto test_fork_handler_results = process_result_queue->pop(m_timeout);

        if  (test_fork_handler_results.empty())
            break; // --->

        for (auto const &handler_result: test_fork_handler_results) {
            paths_left--;
            if (handler_result->node) {
                TResult test_results_fork(handler_result->node);
                for (auto const &test: test_results_fork.Tests)
                    test_results.Tests.add(test);
            }

            auto state = handler_result->state;

            if (state.condition == IProcess::TState::TCondition::RUNNING)
                state = map_path_handler[handler_result->path]->getExitState();

            map_path_handler.erase(handler_result->path);

//            LOGT << "STATE: " << handler_result->path << " " << state.condition << " " << state.exit_code;

            if (checkOneOf(state.condition,
                IProcess::TState::TCondition::DONE,
                IProcess::TState::TCondition::RUNNING))
            {
                LOGI << handler_result->path << ":\n"
                     << handler_result->output;
            } else {
                for (auto const &node: *assertOne(node_test->slice(handler_result->path), "not expected few paths by handler").back()) {
                    TResult::TTests test;
                    test.Path   = handler_result->path + "/" + node->getName();
                    test.Error  = convert<string>(handler_result->state.condition);

                    test_results.Tests.add(test);
                }
                interrupted.push_back(handler_result);
                LOGF << handler_result->path << ":\n"
                     << handler_result->state.condition;
            }
        }
    }

//    LOGT << "session manager finalize ...";
    m_session_manager->finalize();
//    LOGT << "session manager finalize OK";

    if (!interrupted.empty()) {
        LOGF << "\nINTERRUPTED:\n";
        for (auto const &process_result: interrupted)
            LOGF << process_result->state.condition << " "
                 << process_result->path << ":\n\n"
                 << process_result->output;
    }

    if (!map_path_handler.empty()) {
        LOGF << "\nTIMEOUT:\n";
        for (auto const &path_handler: map_path_handler) {
            for (auto const &node: *node_test->slice(path_handler.first).back()) {
                TResult::TTests test;
                test.Path   = path_handler.first + "/" + node->getName();
                test.Error  = "TIMEOUT: process status " + convert<string>(path_handler.second->getExitState().condition);
                test_results.Tests.add(test);
            }
            LOGF << "TIMEOUT "
                 << path_handler.first << ":\n\n"
                 << path_handler.second->getBuffer();
        }
    }

    return test_results;
}


void CTestRunnerFork::scan(
    INodeTest::TSharedPtr const &node,
    string                const &path,
    list<std::string>           &paths)
{
    for (auto const &child: *node) {
        if (child->hasChilds() && !child->begin()->get()->hasChilds())
            paths.push_back(path + "/" + child->getName());
        scan(child, path + "/" + child->getName(), paths);
    }
}


CTestRunnerFork::CTestProtocolHandler::CTestProtocolHandler(
    IProcess::TSharedPtr    const &process,
    string                  const &path,
    IAsyncQueuePusher<TProcessResult::TConstSharedPtr>::TSharedPtr
                            const &process_result_queue)
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

//    LOGT << "\nevent: " <<  event->getType()
//         << "\nfd: "    << (event->getType() == io::IEvent::TType::OPEN ? 0 : event->getStream()->getID())
//         << "\nprocess_state: " << m_state.condition
//         << "\nbuffer:\n" << m_buffer_output;

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
        if (checkOneOf(event->getType(),
            io::IEvent::TType::READ,
            io::IEvent::TType::CLOSE,
            io::IEvent::TType::TIMEOUT) &&
            m_buffer_output             &&
            m_buffer_output->size() > 4 &&
            m_buffer_output->back() == '\n')
        {
            m_process_result = TProcessResult::create(
                TProcessResult {
                    .path   = m_path,
//                    .process = m_process,
                    .state  = m_state,
                    .output = m_buffer_output
                }
            );

            size_t right    = m_buffer_output->size() - 1;
            size_t left     = right;

            while (left > 0 && m_buffer_output->at(left - 1) != '\n')
                left--;

            string size_str(m_buffer_output->begin() + left, m_buffer_output->begin() + right);

//            LOGT << "find size ...";

            if (size_str.find_first_not_of("0123456789") != string::npos)
                return true; // ----->

//            LOGT << "find size OK";

            auto size = convert<uint64_t>(size_str);

//            LOGT << "find json ...";

            if (m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '}'  &&
                m_buffer_output->at(left - size + 2) == '{')
            {
                right   = left + 1;
                left    = left - size + 2;
            } else
                return true; // ----->

//            LOGT << "find json OK";

//            LOGT << "parse json ...";

            string  json(m_buffer_output->begin() + left, m_buffer_output->begin() + right);
            auto    node = m_parser->parse(json);

            m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());
            m_process_result->node = node;
            m_is_finished = true;

//            LOGT << "parse json OK";
//            LOGT << "parsed json for path: " << m_path;
        }
    } catch (std::exception const &e) {
        LOGF << e.what();
        m_is_finished = (m_state.condition != IProcess::TState::TCondition::RUNNING);
    } catch (...) {
        m_is_finished = (m_state.condition != IProcess::TState::TCondition::RUNNING);
    }
//    m_is_finished = true;

//    LOGT << m_buffer_output;

    if (m_is_finished/* && process_result*/)
        m_process_result_queue->push(m_process_result);

    return !m_is_finished; // ----->
}


//void CTestRunnerFork::CTestProtocolHandler::initialize() {
//}


//void CTestRunnerFork::CTestProtocolHandler::finalize() {
////    LOGT << "finalize protocol: " << m_path;
////    if (m_process_result && m_is_finished)
////        m_process_result_queue->push(m_process_result);
//}


io::Buffer::TSharedPtr CTestRunnerFork::CTestProtocolHandler::getBuffer() const {
    return m_buffer_output; // ----->
}


IProcess::TState CTestRunnerFork::CTestProtocolHandler::getExitState() const {
    return m_process->getState(); // ----->
}


} // implementation
} // testing
} // iridium
