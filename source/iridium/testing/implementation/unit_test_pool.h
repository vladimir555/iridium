#ifndef HEADER_UNIT_TEST_POOL_F3B396C8_1F06_4C26_9858_80508C186F2D
#define HEADER_UNIT_TEST_POOL_F3B396C8_1F06_4C26_9858_80508C186F2D


#include "iridium/threading/worker.h"
#include "iridium/io/event_provider.h"
#include "iridium/system/process.h"
#include <string>


namespace iridium {
namespace testing {
namespace implementation {


// result + output text
struct TResult {
    system::IProcess::TState    state;
    std::string                 output;
    std::string                 path;
};


class CUnitTestHandler: public threading::IWorker<std::string, TResult>::IHandler {
public:
    DEFINE_IMPLEMENTATION(CUnitTestHandler)

    CUnitTestHandler(std::string const &app_path);

    // path
    typedef threading::IWorker<std::string, TResult>::TInputItems   TInputItems;
    typedef threading::IWorker<std::string, TResult>::TOutputItems  TOutputItems;

    TOutputItems handle(TInputItems const &paths) override;

private:
    std::string                     m_app_path;
    io::IEventProvider::TSharedPtr  m_event_provider;
};


} // implementation
} // testing
} // iridium


#endif // HEADER_UNIT_TEST_POOL_F3B396C8_1F06_4C26_9858_80508C186F2D
