#include "test_runner_fork.h"

#include "iridium/parsing/implementation/parser_json.h"
#include "iridium/io/implementation/session_manager.h"
#include "iridium/io/implementation/pipe.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/system/implementation/process.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/logging/logger.h"
#include "iridium/items.h"
#include "iridium/assert.h"


using iridium::parsing::implementation::CJSONParser;
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

    int64_t paths_left = paths.size();

    std::list<TProcessResult::TConstSharedPtr> interrupted;

    while (paths_left > 0) {
        auto results = process_result_queue->pop(m_timeout);

        if  (results.empty())
            break; // --->

        for (auto const &result: results) {
            paths_left--;
            if (result->node) {
                TResult test_results_fork(result->node);
                for (auto const &test: test_results_fork.Tests)
                    test_results.Tests.add(test);
            }

            auto state = result->state;

            if (state.condition == IProcess::TState::TCondition::RUNNING)
                state = map_path_handler[result->path]->getExitState();

            map_path_handler.erase(result->path);

            if (checkOneOf(state.condition,
                IProcess::TState::TCondition::DONE,
                IProcess::TState::TCondition::RUNNING))
            {
                LOGI << result->path << ":\n"
                     << result->output;
            } else {
                for (auto const &node: *assertOne(node_test->slice(result->path), "not expected few paths by handler").back()) {
                    TResult::TTests test;
                    test.Path   = result->path + "/" + node->getName();
                    test.Error  = convert<string>(result->state.condition);

                    test_results.Tests.add(test);
                }
                interrupted.push_back(result);
                LOGF << result->path << ":\n"
                     << result->state.condition;
            }
        }
    }

    m_session_manager->finalize();

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
    m_process_result_queue  (process_result_queue),
    m_buffer_output         (io::Buffer::create()),
    m_parser                (CJSONParser::create()),
    m_process_result        (TProcessResult::create( TProcessResult { .path = path } ) )
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::IEvent::TSharedPtr          const &event,
    io::IPipeManager::TSharedPtr    const &pipe_manager)
{
    if (m_process_result->output)
        return false; // ----->

//    LOGT << "\nevent: " <<  event->getType()
//         << "\nfd: "    << (event->getType() == io::IEvent::TType::OPEN ? 0 : event->getStream()->getID())
//         << "\nprocess_state: " << m_state.condition
//         << "\nbuffer:\n" << m_buffer_output;

    m_process_result->state = m_process->getState();

    if (event->getType() == io::IEvent::TType::OPEN) {
        pipe_manager->createPipe("process");
        pipe_manager->updateReader("process", std::dynamic_pointer_cast<io::IStreamReader>(event->getStream()));
        pipe_manager->updateWriter("process", CStreamWriterBuffer::create(m_buffer_output));
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
            size_t right = m_buffer_output->size() - 1;
            size_t left  = right;

            while (left > 0 && m_buffer_output->at(left - 1) != '\n')
                left--;

            string size_str(m_buffer_output->begin() + left, m_buffer_output->begin() + right);

            if (size_str.find_first_not_of("0123456789") != string::npos)
                return true; // ----->

            auto size = convert<uint64_t>(size_str);

            if (m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '\n' &&
                m_buffer_output->at(--left) == '}'  &&
                m_buffer_output->at(left - size + 2) == '{')
            {
                right   = left + 1;
                left    = left - size + 2;
            } else
                return true; // ----->

            string  json(m_buffer_output->begin() + left, m_buffer_output->begin() + right);
            auto    node = m_parser->parse(json);

            m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());
            m_process_result->node      = node;
            m_process_result->output    = m_buffer_output;
        }
    } catch (std::exception const &e) {
        LOGF << e.what();
        if (m_process_result->state.condition != IProcess::TState::TCondition::RUNNING)
            m_process_result->output = m_buffer_output;;
    } catch (...) {
        if (m_process_result->state.condition != IProcess::TState::TCondition::RUNNING)
            m_process_result->output = m_buffer_output;;
    }

    // detect crash
    if (!checkOneOf(m_process_result->state.condition,
        IProcess::TState::TCondition::DONE,
        IProcess::TState::TCondition::RUNNING))
    {
        m_process_result->output = m_buffer_output;
    }

    if (m_process_result->output)
        m_process_result_queue->push(m_process_result);

    return !m_process_result->output; // ----->
}


io::Buffer::TSharedPtr CTestRunnerFork::CTestProtocolHandler::getBuffer() const {
    return m_buffer_output; // ----->
}


IProcess::TState CTestRunnerFork::CTestProtocolHandler::getExitState() const {
    return m_process->getState(); // ----->
}


} // implementation
} // testing
} // iridium
