#ifndef HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
#define HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7


#include "iridium/parsing/node_type.h"
#include "test.h"


namespace iridium {
namespace testing {


class ITestRunner {
public:
    DEFINE_INTERFACE(ITestRunner)
    typedef parsing::INodeType<ITest *> INodeTest;

    struct TResult {
        std::list<std::string> passed;
        std::list<std::string> failed;
    };

    virtual TResult run(INodeTest::TSharedPtr const &node_test) = 0;
};


} // testing
} // iridium


DEFINE_CONVERT(std::string, iridium::testing::ITestRunner::INodeTest::TConstSharedPtr)
DEFINE_CONVERT(std::string, iridium::testing::ITestRunner::INodeTest::TSharedPtr)



#endif // HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
