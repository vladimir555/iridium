#ifndef HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA
#define HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA


#include "iridium/testing/test_runner.h"


namespace iridium {
namespace testing {
namespace implementation {


class CTestRunnerRaw: public ITestRunner {
public:
    CTestRunnerRaw() = default;
    DEFINE_IMPLEMENTATION(CTestRunnerRaw)

    TTestResult run(INodeTest::TSharedPtr const &node_test) override;

private:
    void run(
        TTestResult                   &test_results,
        INodeTest::TSharedPtr   const &node,
        std::string             const &path = "");
};


} // implementation
} // testing
} // iridium


#endif // HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA
