#ifndef HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
#define HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7


#include "iridium/parsing/node_type.h"
#include "test.h"

#include "iridium/parsing/serialization/node.h"


namespace iridium {
namespace testing {


DEFINE_ROOT_NODE_BEGIN(TestResult)
    DEFINE_NODE_LIST_BEGIN(Test)
        DEFINE_ATTRIBUTE(std::string, Path, "")
        DEFINE_ATTRIBUTE(std::string, Error, "")
        DEFINE_ATTRIBUTE(std::string, Output, "")
    DEFINE_NODE_LIST_END(Test)
DEFINE_ROOT_NODE_END()


class ITestRunner {
public:
    DEFINE_INTERFACE(ITestRunner)
    typedef parsing::INodeType<ITest *> INodeTest;

//    struct TResult {
//        std::list<std::string> passed;
//        std::list<std::string> failed;
//    };

    virtual TTestResult run(INodeTest::TSharedPtr const &node_test) = 0;
};


} // testing
} // iridium


DEFINE_CONVERT(std::string, iridium::testing::ITestRunner::INodeTest)


#endif // HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
