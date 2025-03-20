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
    milliseconds    const &timeout,
    bool            const &is_serial)
:
    m_app_path          (app_path),
    m_timeout           (timeout),
    m_is_serial         (is_serial),
    m_session_manager   (CSessionManager::create())
{}


TResult CTestRunnerFork::run(INodeTest::TSharedPtr const &node_test) {
    TResult test_results;
    m_session_manager->initialize();

    list<string> paths;
    scan(node_test, "", paths);

    struct TFork {
        CProcessStream::TSharedPtr       process;
        CTestProtocolHandler::TSharedPtr handler;
    };

    std::unordered_map<std::string, TFork> map_path_fork;
    auto process_result_queue = CAsyncQueue<TProcessResult::TConstSharedPtr>::create();

    for (auto const &path: paths) {
        auto process = CProcessStream::create(m_app_path, "run --mode=raw --print-result=json " + path);
        auto handler = CTestProtocolHandler::create(process, path, process_result_queue, m_timeout);

        map_path_fork[path] = TFork { process, handler };

        if (!m_is_serial)
            m_session_manager->manage(process, handler);
    }

    std::list<TProcessResult::TConstSharedPtr> interrupted;

    // TODO: exception handling
    while (!map_path_fork.empty()) {
        for (auto const &i: map_path_fork)
            LOGT << "path left: " << i.first;

        if (m_is_serial && !map_path_fork.empty()) {
            auto fork = map_path_fork.begin()->second;
            m_session_manager->manage(fork.process, fork.handler);
        }

        auto results = process_result_queue->pop(m_timeout);
        //LOGT << "wait, paths_left: " << paths_left << " OK, results: " << results.size();

        for (auto const &result: results) {
            if (result->node) {
//                LOGT << result->node;
                TResult test_results_fork(result->node);
                for (auto const &test: test_results_fork.Tests)
                    test_results.Tests.add(test);
            }

            auto state = result->state;

            if (state.condition == IProcess::TState::TCondition::RUNNING)
                state = map_path_fork[result->path].handler->getExitState();

            map_path_fork.erase(result->path);

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
                     << result->state.condition << "\n\n";
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

    if (!map_path_fork.empty()) {
        LOGF << "\nTIMEOUT:\n";
        for (auto const &path_fork: map_path_fork) {
            for (auto const &node: *node_test->slice(path_fork.first).back()) {
                TResult::TTests test;
                test.Path   = path_fork.first + "/" + node->getName();
                // TODO: getExitState() thread sync
                test.Error  = "TIMEOUT: process status " + convert<string>(path_fork.second.handler->getExitState().condition);
                test_results.Tests.add(test);
            }
            LOGF << "TIMEOUT "
                 << path_fork.first << ":\n\n"
                 << path_fork.second.handler->getBuffer();
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
    IProcess::TSharedPtr
        const &process,
    string
        const &path,
    IAsyncQueuePusher<TProcessResult::TConstSharedPtr>::TSharedPtr
        const &process_result_queue,
    std::chrono::milliseconds
        const &timeout)
:
    m_process               (process),
    m_process_result_queue  (process_result_queue),
    m_buffer_output         (io::Buffer::create()),
    m_parser                (CJSONParser::create()),
    m_process_result        (TProcessResult::create( TProcessResult { path } ) ),
    m_timeout               (timeout)
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::Event::TSharedPtr           const &event,
    io::IPipeManager::TSharedPtr    const &pipe_manager)
{
    if (m_process_result->output) {
        //LOGT << "return false";
        return false; // ----->
    }

    m_process_result->state = m_process->getState();

    //LOGT << "\nevent:           "   <<  event->operation
    //     << "\nfd:              "   << (event->operation == io::Event::TOperation::OPEN ? 0 :
    //                                    event->stream->getHandles().front())
    //     << "\nprocess_state:   "   << m_process_result->state.condition
    //     << "\nbuffer:\n"           << m_buffer_output;

    //if (m_process_result->state.condition == IProcess::TState::TCondition::DONE) {
    //    LOGT << "DONE";
    //}

    if (event->operation == io::Event::TOperation::OPEN) {
        static std::string const DEFAULT_PIPE_NAME = "process";
        pipe_manager->createPipe(DEFAULT_PIPE_NAME);
        pipe_manager->updatePipe(DEFAULT_PIPE_NAME, 
            std::dynamic_pointer_cast<io::IStreamReader>(event->stream), 
            CStreamWriterBuffer::create(m_buffer_output));

//        m_time_end = m_timeout + std::chrono::system_clock::now();
//        LOGT << "start: " << m_process_result->path;
//        LOGT << "return true";
        return true; // ----->
    }

    //if (m_buffer_output && !m_buffer_output->empty())
    //    LOGT << "back: " << (int)m_buffer_output->back();

//    if (m_time_end < std::chrono::system_clock::now()) {
//        LOGT << "stop:  " << m_process_result->path;
//        m_process->finalize();
//        m_process_result->state     = m_process->getState();
////        m_process_result->output    = m_buffer_output;
//    }

    bool result = true;

    try {
        if (
            //m_process_result->state.condition != IProcess::TState::TCondition::RUNNING ||
            checkOneOf(event->operation,
                io::Event::TOperation::READ,
                io::Event::TOperation::EOF_,
                io::Event::TOperation::CLOSE,
                io::Event::TOperation::TIMEOUT) &&
                m_buffer_output             &&
                m_buffer_output->size() > 4 &&
                checkOneOf(m_buffer_output->back(), uint8_t('\n'), uint8_t('\r'), uint8_t('\x00')))
        {
            size_t right = m_buffer_output->size() - 1;
            while (right > 0 && checkOneOf(m_buffer_output->at(right), uint8_t('\n'), uint8_t('\r'), uint8_t('\x00')))
                right--;

            size_t left  = right;

            while (left  > 0 && !checkOneOf(m_buffer_output->at(left),  uint8_t('\n'), uint8_t('\r'), uint8_t('\x00')))
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
                //LOGT << "endlines_count: " << endlines_count;
                
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

                    //LOGT << "right - left = " << right - left << ", size = " << size;
                    if (right - left == size) {
                        string  json(m_buffer_output->begin() + left, m_buffer_output->begin() + right);
                        auto    node = m_parser->parse(json);
                        
                        m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());
                        m_process_result->node      = node;
                        m_process_result->output    = m_buffer_output;

                        LOGT << "json:\n"   << json;
                        LOGT << "node:\n"   << node;
                        //LOGT << "output:\n" << m_buffer_output;
                        LOGT << "set result = false";
                        result = false;
                    }
                }
            }
        }

//        if (!m_process_result->output && event->operation == io::Event::TOperation::CLOSE)
//            throw std::runtime_error("unexpected closing console pipe");

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
    //LOGT << "process state: " << m_process_result->state.condition;
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
        //LOGT << "empty output, close, m_process_result->state.condition: "
        //     << m_process_result->state.condition;
        m_process_result->output    = io::Buffer::create("empty process output");
        m_process_result->state     = m_process->getState();
    }

//        || (event->operation   == io::Event::TOperation::CLOSE &&
//            event->status      == io::Event::TStatus::END));

    LOGT
        << "\nresult:   "           << result
        << "\ncond:     "           << m_process_result->state.condition
        << "\nevent fd: "           << event->stream->getHandles()
        << "\nevent operation: "    << event->operation
        << "\nevent status   : "    << event->status
        << "\nBUFFER_BEGIN:\n"      << m_buffer_output << "\nBUFFER_END\n";

    if (m_process_result->output)
        m_process_result_queue->push(m_process_result);
    
    LOGT << "protocol return: " << result;

    return result; // ----->
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
