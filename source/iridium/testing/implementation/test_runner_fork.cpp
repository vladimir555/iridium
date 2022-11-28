#include "test_runner_fork.h"

#include "iridium/io/implementation/session_manager.h"
#include "iridium/io/implementation/pipe.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/system/implementation/process.h"

#include "iridium/logging/logger.h"

#include "iridium/items.h"


using iridium::io::implementation::CSessionManager;
using iridium::io::implementation::CPipe;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::system::implementation::CProcessStream;

using std::string;
using std::list;
using std::chrono::milliseconds;


namespace iridium {
namespace testing {
namespace implementation {


CTestRunnerFork::CTestRunnerFork(
    std::string     const &app_path,
    size_t          const &processes_count,
    milliseconds    const &timeout)
:
    m_app_path          (app_path),
    m_processes_count   (processes_count),
    m_timeout           (timeout),
    m_session_manager   (CSessionManager::create())
{}


CTestRunnerFork::TResult CTestRunnerFork::run(INodeTest::TSharedPtr const &node_test) {
    TResult result = {};
    m_session_manager->initialize();

    list<string> paths;
    scan(node_test, "", paths);

    std::unordered_map<std::string, CTestProtocolHandler::TSharedPtr> map_path_handler;

//    int count = 2;

//    for (auto const &path: paths)
    string path = "/threading/worker.cpp";
    {
        LOGT << path;

        auto process    = CProcessStream::create(m_app_path, "run --raw " + path);
        auto handler    = CTestProtocolHandler::create();

        map_path_handler[path] = handler;
        m_session_manager->manage(process, handler);

//        if (--count < 1)
//            break;
    }

    if (!m_session_manager->wait(std::chrono::seconds(10)))
        LOGW << "tests timeout";

    m_session_manager->finalize();

    for (auto const &path_handler: map_path_handler)
        LOGT << "----- " << path_handler.first << ":\n"
             << *path_handler.second->getBuffer() << "\n-----"
             << ", size: " << path_handler.second->getBuffer()->size();

    return result;
}


void CTestRunnerFork::scan(
    INodeTest::TSharedPtr const &node,
    string                const &path,
    list<std::string>           &paths)
{
    for (auto const &child: *node) {
        if (child->hasChilds() && !child->begin()->get()->hasChilds()) {
            paths.push_back(path + "/" + child->getName());
            LOGT << path + "/" + child->getName();
        }
        scan(child, path + "/" + child->getName(), paths);
    }
}


bool CTestRunnerFork::CTestProtocolHandler::control(
    io::IEvent::TSharedPtr const &event,
    std::unordered_map<int, io::IPipe::TSharedPtr> &pipes)
{
    LOGT << __FUNCTION__ << ", fd: " << event->getStream()->getID() << " event: " << event->getType();
//    if (m_buffer_output)
//        LOGT << "buffer:\n" << *m_buffer_output;
    if (event->getType() == io::IEvent::TType::OPEN && !m_stream_output) {
        auto pipe = CPipe::create();
        m_buffer_output = io::Buffer::create();
        m_stream_output = CStreamWriterBuffer::create(m_buffer_output);
        pipe->set(std::dynamic_pointer_cast<io::IStreamReader>(event->getStream()), m_stream_output);
        pipes[0] = pipe;

//        LOGT << "1 true";
        return true;
    }

//    if (event->getType() == io::IEvent::TType::READ) {
//        LOGT << "2 true";
//        return true;
//    }

    bool result = checkOneOf(event->getType(), io::IEvent::TType::READ, io::IEvent::TType::WRITE, io::IEvent::TType::OPEN);
    LOGT << __FUNCTION__ << ", result: " << result;
    return result;
}


io::Buffer::TSharedPtr CTestRunnerFork::CTestProtocolHandler::getBuffer() const {
    return m_buffer_output;
}


} // implementation
} // testing
} // iridium
