// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
#define HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7


#include "iridium/parsing/node_type.h"
#include "test.h"

#include "iridium/parsing/serialization/node.h"


namespace iridium::testing {


DEFINE_ROOT_NODE_BEGIN(TestRunResult)
    // USAGE:
    // Test Suite (Path + Output)
    // OR
    // Test Case  (Path + Line + Name + Error)
    DEFINE_NODE_LIST_BEGIN(TestCases)
        DEFINE_ATTRIBUTE(std::string, Path, "")
        DEFINE_ATTRIBUTE(size_t,      Line, 0)
        DEFINE_ATTRIBUTE(std::string, Name, "")
        DEFINE_ATTRIBUTE(std::string, Error, "")
        DEFINE_ATTRIBUTE(std::string, Output, "")
    DEFINE_NODE_LIST_END(TestCases)
DEFINE_ROOT_NODE_END()


class ITestRunner {
public:
    DEFINE_INTERFACE(ITestRunner)
    typedef parsing::INodeType<IUnitTestCase *> IUnitTestCaseNode;
    virtual TTestRunResult run(IUnitTestCaseNode::TSharedPtr const &unit_test_case_tree) = 0;
};


} // namespace iridium::testing


DEFINE_CONVERT(std::string, iridium::testing::ITestRunner::IUnitTestCaseNode)


#endif // HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
