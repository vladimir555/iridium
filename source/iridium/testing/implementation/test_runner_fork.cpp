#include "test_runner_fork.h"

#include "iridium/parsing/implementation/json_parser.h"
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
using std::chrono::seconds;
using std::chrono::system_clock;


namespace iridium::testing::implementation {


std::chrono::system_clock::duration const CTestRunnerFork::DEFAULT_TIMEOUT = seconds(10);


CTestRunnerFork::CTestRunnerFork(
    std::string
        const &app_path,
    std::chrono::system_clock::duration
        const &timeout,
    bool
        const &is_serial)
:
    m_app_path
        (app_path),
    m_timeout
        (timeout),
    m_is_serial
        (is_serial),
    m_session_manager
        (CSessionManager::create())
{
    m_session_manager->initialize();
}


CTestRunnerFork::~CTestRunnerFork() {
    m_session_manager->finalize();
}


TTestRunResult CTestRunnerFork::run(IUnitTestCaseNode::TSharedPtr const &test_node) {
    struct TFork {
        CProcessStream::TSharedPtr
            process;
        CTestProtocolHandler::TSharedPtr
            handler;
        std::chrono::system_clock::time_point
            expiration_time;
    };

    TTestRunResult
        test_results;
    list<string>
        paths;
    std::map<std::string, TFork>
        map_path_fork;
    int64_t const
        forks_limit = m_is_serial ? 1 : std::thread::hardware_concurrency();
    int64_t
        forks_count = 0;

    auto process_result_queue =
        CAsyncQueue<TProcessResult::TConstSharedPtr>::create();

    scan(test_node, "", paths);

    do {
        list<string>
            processing_paths;

        // start forks
        for (; forks_count < forks_limit && !paths.empty(); forks_count++) {
            auto path = paths.front();

            processing_paths.push_back(path);
            paths.pop_front();

            auto process =
                CProcessStream::create(m_app_path, "run --mode=raw --print-result=json " + path);
            auto handler =
                CTestProtocolHandler::create(process, path, process_result_queue, m_timeout);

            map_path_fork[path] = TFork {
                process,
                handler,
                system_clock::now() + m_timeout
            };

            // LOGT << "start: " << process->getURI();
            m_session_manager->manage(process, handler);
        }

        // LOGT << "forks_count 1: " << forks_count;
        auto process_results = process_result_queue->pop(m_timeout);
        forks_count -= process_results.size();
        // LOGT << "forks_count 2: " << forks_count;

        // for (auto const &process_result: process_results) {
        //     LOGT << process_result->path << ":" << process_result->node;
        // }

        // timeout
        bool is_interrupted = false;
        auto now = system_clock::now();

        for (auto const &path_fork: map_path_fork) {
            if (path_fork.second.expiration_time < now &&
                path_fork.second.process->getState().condition == IProcess::TState::TCondition::RUNNING)
            {
                // LOGT << "interrupt: " << path_fork.second.process->getURI();
                path_fork.second.process->sendSignal(IProcess::TSignal::INTERRUPT);
                is_interrupted = true;
            }
        }

        std::unordered_set<TProcessResult::TConstSharedPtr>
            timed_out_process_results;

        if (is_interrupted) {
            auto interrupted_process_results = process_result_queue->pop(m_timeout);
            forks_count -= interrupted_process_results.size();
            // LOGT << "forks_count 3: " << forks_count;

            timed_out_process_results.insert(
                interrupted_process_results.begin(),
                interrupted_process_results.end());

            process_results.insert(
                process_results.end(),
                interrupted_process_results.begin(),
                interrupted_process_results.end());
        }

        // LOGT << "process_results: " << process_results.size();
        // LOGT << "forks_count: " << forks_count;

        for (auto const &process_result: process_results) {
            // LOGT << process_result->path << ", state: " << process_result->state;

            if (checkOneOf(
                    process_result->state.condition,
                    IProcess::TState::TCondition::DONE,
                    IProcess::TState::TCondition::RUNNING)
                &&  process_result->node)
            {
                LOGI << process_result->path << ":\n"
                     << process_result->output;

                TTestRunResult test_run_result(process_result->node);

                for (auto const &result: test_run_result.TestCases)
                    test_results.TestCases.add(result);

                // LOGT << "erase 1: " << process_result->path;
                map_path_fork.erase(process_result->path);
            } else {
                bool is_timeout =
                    timed_out_process_results.count(process_result);
                auto test_file_node =
                    assertOne(
                        test_node->slice(process_result->path),
                        "unexpected few paths by handler");
                auto test_file_path =
                    process_result->path;

                // LOGT << is_timeout;
                string error;

                if (!process_result->node) {
                    if (process_result->state.condition == IProcess::TState::TCondition::DONE) {
                        error = "internal error: process '" + process_result->path +
                            "' stdout json not parsed, output:\n"
                            + convert<string>(process_result->output);
                    } else {
                        if (is_timeout)
                            error = "TIMEOUT " + convert<string>(m_timeout) + ", ";

                        error += convert<string>(process_result->state.condition);

                        LOGE
                            << "\n\n" << error << ":\n"
                            << process_result->path << ":\n"
                            << "-----\n"
                            << process_result->output
                            << "-----";
                    }
                }

                if (error.empty())
                    error = "no error text";

                for (auto const &test_case_node: *assertOne(test_node->slice(test_file_path), "unexpected few copies of test suites")) {
                    TTestRunResult::TTestCases test_case;

                    test_case.Path   = process_result->path;
                    test_case.Error  = error;
                    test_case.Name   = test_case_node->getName();

                    test_results.TestCases.add(test_case);
                }

                // LOGT << "erase 2: " << process_result->path;
                map_path_fork.erase(process_result->path);
            }
        }

        if (forks_count > forks_limit)
            throw std::runtime_error("internal testing error: forks limit " +
                convert<string>(forks_limit) + " exceeded");

        // LOGT << "forks_count: " << forks_count;
    } while (!paths.empty() || !map_path_fork.empty());

    // LOGT << "test_results cases size: " << test_results.TestCases.size();
    return test_results; // ----->
}


void CTestRunnerFork::scan(
    IUnitTestCaseNode::TSharedPtr
        const &node,
    string
        const &path,
    list<std::string>
              &paths)
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
    std::chrono::system_clock::duration
        const &)
:
    m_process
        (process),
    m_process_result_queue
        (process_result_queue),
    // m_buffer_output
    //     (io::Buffer::create()),
    m_parser
        (CJSONParser::create()),
    m_process_result
        (TProcessResult::create( TProcessResult { path, {}, {}, {} } ) )
//    m_timeout
//        (timeout)
{}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::Event::TSharedPtr
        const &event,
    io::IPipeManager::TSharedPtr
        const &pipe_manager)
{
    // LOGT
    //     << "\nevent: " << event
    //     << "\nstate: " << m_process_result->state.condition;

    if (!m_buffer_output && event->operation != io::Event::TOperation::OPEN) {
        // LOGT << "skip, return true";
        return true; // ----->
    } else {
        // LOGT << "inited";
    }

    if (m_process_result->output || event->operation == io::Event::TOperation::ERROR_) {
        //LOGT << "return false";
        return false; // ----->
    }

    m_process_result->state = m_process->getState();

    if (event->operation == io::Event::TOperation::OPEN) {
        // todo: fix event state machine, repeating open begin
        if (m_buffer_output)
            return true; // ----->

        m_buffer_output = io::Buffer::create();

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

    bool result = m_process_result->state.condition == IProcess::TState::TCondition::RUNNING;
    // LOGT << "result: " << result << ", condition: " << m_process_result->state.condition;

    try {
        // if (event->operation == io::Event::TOperation::ERROR_) {
        //     throw std::runtime_error("pipe error event");
        // }

        if (
            //m_process_result->state.condition != IProcess::TState::TCondition::RUNNING ||
            checkOneOf(
                event->operation,
                io::Event::TOperation::READ,
                io::Event::TOperation::CLOSE,
                io::Event::TOperation::TIMEOUT) &&
            m_buffer_output             &&
            m_buffer_output->size() > 4 &&
            checkOneOf(
                m_buffer_output->back(),
                uint8_t('\n'), uint8_t('\r'), uint8_t('\x00')))
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

//                        m_buffer_output->erase(m_buffer_output->begin() + left, m_buffer_output->end());
                        m_buffer_output->resize(left);
                        m_process_result->node      = node;
                        m_process_result->output    = m_buffer_output;

                        // LOGT << "json:\n"   << json;
                        // LOGT << "node:\n"   << node;
                        //LOGT << "output:\n" << m_buffer_output;
                        result = false;
                    }
                }
            }
        }

// freebsd posix bug handling
#ifdef FREEBSD_PLATFORM
        if (!m_process_result->output && event->operation == io::Event::TOperation::CLOSE) {
            // todo: timeout
            while (m_process_result->state.condition == IProcess::TState::TCondition::RUNNING) {
                m_process_result->state = m_process->getState();
                // LOGT << "update state: " << m_process_result->state;
            }

            result = false;

            if (m_process_result->state.exit_code && *m_process_result->state.exit_code == 1)
                m_process_result->state.condition = IProcess::TState::TCondition::CRASHED;
        }
        // LOGT << "fixed freebsd state: " << m_process_result->state;
#endif // FREEBSD_PLATFORM


        if (!m_process_result->output && event->operation == io::Event::TOperation::CLOSE)
           throw std::runtime_error("unexpected closing console pipe, event: " + convert<string>(event));

        if (!m_process_result->output && event->operation == io::Event::TOperation::TIMEOUT)
           throw std::runtime_error("console pipe timeout, event: " + convert<string>(event));

    } catch (std::exception const &e) {
        LOGF << e.what() << + ", state: " + convert<string>(m_process_result->state);
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
        // m_process_result->state     = m_process->getState();
        // m_process_result->
    }

    // todo: check / fix condition
    if(!result && (m_process_result->node || m_process_result->output)) {
        // LOGT << "push result: " << m_process_result->path << "\nevent: " << event;
        m_process_result_queue->push(m_process_result);
    }

    // LOGT << "protocol return: " << result;

    return result; // ----->
}


io::Buffer::TSharedPtr CTestRunnerFork::CTestProtocolHandler::getBuffer() const {
    return m_buffer_output; // ----->
}


IProcess::TState CTestRunnerFork::CTestProtocolHandler::getExitState() const {
    return m_process->getState(); // ----->
}


} // iridium::testing::implementation
