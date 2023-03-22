#ifndef HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA
#define HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA


#include "iridium/testing/test_runner.h"
#include "iridium/io/session.h"
#include "iridium/system/process.h"
#include "iridium/threading/async_queue.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace testing {
namespace implementation {


class CTestRunnerFork: public ITestRunner {
public:
    static std::chrono::seconds const DEFAULT_TIMEOUT;
    CTestRunnerFork(
        std::string                 const &app_name,
        std::chrono::milliseconds   const &timeout = DEFAULT_TIMEOUT
    );
    DEFINE_IMPLEMENTATION(CTestRunnerFork)

    TResult run(INodeTest::TSharedPtr const &node_test) override;
private:
    struct TProcessResult {
        DEFINE_CREATE(TProcessResult)
        std::string                 path;
        system::IProcess::TState    state;
        io::Buffer::TSharedPtr      output;
        parsing::INode::TSharedPtr  node;
    };

    class CTestProtocolHandler: public io::IProtocol {
    public:
        DEFINE_IMPLEMENTATION(CTestProtocolHandler)
        CTestProtocolHandler(
            system::IProcess::TSharedPtr    const &process,
            std::string                     const &path,
            threading::IAsyncQueuePusher<TProcessResult::TConstSharedPtr>::TSharedPtr
                                            const &process_result_queue);

        bool control(
            io::IEvent::TSharedPtr          const &event,
            io::IPipeManager::TSharedPtr    const &pipe_manager) override;

        io::Buffer::TSharedPtr      getBuffer() const;
        system::IProcess::TState    getExitState() const;

    private:
        system::IProcess::TSharedPtr    m_process;
        threading::IAsyncQueuePusher<TProcessResult::TConstSharedPtr>::TSharedPtr
                                        m_process_result_queue;
        io::Buffer::TSharedPtr          m_buffer_output;
        parsing::IParser::TSharedPtr    m_parser;
        TProcessResult::TSharedPtr      m_process_result;
    };

    void scan(
        INodeTest::TSharedPtr const &node,
        std::string           const &path,
        std::list<std::string>      &paths);

    std::string                     m_app_path;
    std::chrono::milliseconds       m_timeout;
    io::ISessionManager::TSharedPtr m_session_manager;
};


} // implementation
} // testing
} // iridium


#endif // HEADER_TEST_RUNNER_FORK_7CD34E81_3DBC_4751_AAA2_AEC4EFBA58AA
