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

    // TODO: exception handling
    while (paths_left > 0) {
//        LOGT << "wait, paths_left: " << paths_left << " ...";
        auto results = process_result_queue->pop(m_timeout);
//        LOGT << "wait, paths_left: " << paths_left << " OK";

        if  (results.empty())
            break; // --->

        for (auto const &result: results) {
            paths_left--;
            if (result->node) {
//                LOGT << result->node;
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
                for (auto const &node: *assertOne(node_test->slice(result->path), "unexpected few paths by handler").back()) {
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
                // TODO: getExitState() thread sync
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
    m_process_result        (TProcessResult::create( TProcessResult { path } ) )
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::Event::TSharedPtr           const &event,
    io::IPipeManager::TSharedPtr    const &pipe_manager)
{
    if (m_process_result->output) {
        LOGT << "return false";
        return false; // ----->
    }

//    LOGT << "\nevent:           "   <<  event->getType()
//         << "\nfd:              "   << (event->getType() == io::IEvent::TType::OPEN ? 0 : event->getStream()->getID())
//         << "\nprocess_state:   "   << m_process_result->state.condition
//         << "\nbuffer:\n"           << m_buffer_output;

    m_process_result->state = m_process->getState();

    if (event->operation == io::Event::TOperation::OPEN) {
        static std::string const DEFAULT_PIPE_NAME = "process";
        pipe_manager->createPipe(DEFAULT_PIPE_NAME);
        pipe_manager->updatePipe(DEFAULT_PIPE_NAME, std::dynamic_pointer_cast<io::IStreamReader>(event->stream), CStreamWriterBuffer::create(m_buffer_output));
//        LOGT << "return true";
        return true; // ----->
    }

    try {
        if (
//            m_process_result->state.condition != IProcess::TState::TCondition::RUNNING &&
            checkOneOf(event->operation,
            io::Event::TOperation::EOF_,
            io::Event::TOperation::CLOSE,
            io::Event::TOperation::TIMEOUT) &&
            m_buffer_output             &&
            m_buffer_output->size() > 4 &&
            m_buffer_output->back() == '\n')
        {
            size_t right = m_buffer_output->size() - 1;
            while (right > 0 &&  checkOneOf(m_buffer_output->at(right), uint8_t('\n'), uint8_t('\r')))
                right--;

            size_t left  = right;

            while (left  > 0 && !checkOneOf(m_buffer_output->at(left),  uint8_t('\n'), uint8_t('\r')))
                left--;

            string size_str(m_buffer_output->begin() + left + 1, m_buffer_output->begin() + right + 1);

            //LOGT << "size_str: '" << size_str << "'";

            if (size_str.find_first_not_of("0123456789") == string::npos) {
                auto    size = convert<uint64_t>(size_str);
                size_t  endlines_count = 0;
                
                while (left > 0) {
                    if (m_buffer_output->at(left) == '\n')
                        endlines_count++;
                    else
                        if (m_buffer_output->at(left) != '\r')
                            break;
                    left--;
                }
                
                if (endlines_count == 2 && m_buffer_output->at(left) == '}') {
                    right = left + 2;
                    
                    size_t brackets_count = 1;
                    while (brackets_count > 0 && --left > 0) {
                        if (m_buffer_output->at(left) == '}')
                            brackets_count++;
                        
                        if (m_buffer_output->at(left) == '{')
                            brackets_count--;
                        
                        if (m_buffer_output->at(left) == '\r')
                            size++;
                    }
                    
                    if (right - left == size) {
                        string  json(m_buffer_output->begin() + left, m_buffer_output->begin() + right);
                        auto    node = m_parser->parse(json);
                        
                        m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());
                        m_process_result->node      = node;
                        m_process_result->output    = m_buffer_output;
                        
//                        LOGT << "node:\n"   << node;
//                        LOGT << "output:\n" << m_buffer_output;
                    }
                }
            }
        }
    } catch (std::exception const &e) {
        LOGF << e.what();
        if (m_process_result->state.condition != IProcess::TState::TCondition::RUNNING)
            m_process_result->output = m_buffer_output;
    } catch (...) {
        if (m_process_result->state.condition != IProcess::TState::TCondition::RUNNING)
            m_process_result->output = m_buffer_output;
    }
    
//    LOGT << "output:\n" << m_buffer_output;

    // detect crash
    LOGT << "process state: " << m_process_result->state.condition;
    if (!checkOneOf(m_process_result->state.condition,
        IProcess::TState::TCondition::DONE,
        IProcess::TState::TCondition::RUNNING))
    {
        m_process_result->output = m_buffer_output;
    }

    if(!m_process_result->output &&
        event->operation == io::Event::TOperation::CLOSE
//        && m_process_result->state.condition == IProcess::TState::TCondition::CRASHED
       )
    {
        LOGT << "empty output, close, m_process_result->state.condition: "
             << m_process_result->state.condition;
        m_process_result->output    = io::Buffer::create("empty process output");
        m_process_result->state     = m_process->getState();
    }
    
//    LOGT << "return " << bool(!m_process_result->output);
//    if (!m_process_result->output)
//        LOGT << "buffer:\n" << m_buffer_output
//             << "\ncond:  " << m_process_result->state.condition
//             << "\nevent: " << event->getType();
    
    if (m_process_result->output)
        m_process_result_queue->push(m_process_result);
    
    LOGT << "protocol return: " << !m_process_result->output;
    return !m_process_result->output;
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
