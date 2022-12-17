#ifndef HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA
#define HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA


#include "iridium/testing/test_runner.h"
#include "iridium/io/session.h"
#include "iridium/system/process.h"


namespace iridium {
namespace testing {
namespace implementation {


class CTestRunnerFork: public ITestRunner {
public:
    CTestRunnerFork(
        std::string const &app_name,
        size_t      const &process_count,
        std::chrono::milliseconds const &timeout
    );
    DEFINE_IMPLEMENTATION(CTestRunnerFork)

    TResult run(INodeTest::TSharedPtr const &node_test) override;
private:
    class CTestProtocolHandler: public io::IProtocol {
    public:
        DEFINE_IMPLEMENTATION(CTestProtocolHandler)
        CTestProtocolHandler(system::IProcess::TSharedPtr const &process);
        bool control(
            io::IEvent::TSharedPtr const &event,
            io::IPipeManager::TSharedPtr const &pipe_manager) override;
        io::Buffer::TSharedPtr getBuffer() const;
        system::IProcess::TState getExitState() const;
    private:
        system::IProcess::TSharedPtr    m_process;
        system::IProcess::TState        m_state;
        io::IStreamWriter::TSharedPtr   m_stream_output;
        io::Buffer::TSharedPtr          m_buffer_output;
    };

    void scan(
        INodeTest::TSharedPtr const &node,
        std::string           const &path,
        std::list<std::string>      &paths);

    std::string                     m_app_path;
    size_t                          m_processes_count;
    std::chrono::milliseconds       m_timeout;
    io::ISessionManager::TSharedPtr m_session_manager;
};


} // implementation
} // testing
} // iridium


#endif // HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA
