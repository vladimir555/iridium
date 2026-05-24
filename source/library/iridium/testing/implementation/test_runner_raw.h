// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA
#define HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA


#include "iridium/testing/test_runner.h"


namespace iridium::testing::implementation {


class CTestRunnerRaw: public ITestRunner {
public:
    CTestRunnerRaw() = default;
    DEFINE_IMPLEMENTATION(CTestRunnerRaw)

    TTestRunResult run(IUnitTestCaseNode::TSharedPtr const &node_test) override;

private:
    void run(
        TTestRunResult
                  &test_results,
        IUnitTestCaseNode::TSharedPtr
            const &unit_test_case_tree,
        std::string
            const &path = "");
};


} // iridium::testing::implementation


#endif // HEADER_TEST_RUNNER_RAW_1EC41C63_C147_4CB5_B9C0_754A6A0C85DA
