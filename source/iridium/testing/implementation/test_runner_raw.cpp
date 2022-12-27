#include "test_runner_raw.h"

#include "iridium/testing/unit_test.h"
#include "iridium/logging/logger.h"

#include <string>


using std::string;


namespace iridium {
namespace testing {
namespace implementation {


TTestResult CTestRunnerRaw::run(INodeTest::TSharedPtr const &node_test) {
    TTestResult result;
    run(result, node_test);
    return result;
}


void CTestRunnerRaw::run(
    TTestResult                   &results,
    INodeTest::TSharedPtr   const &node,
    std::string             const &path)
{
    for (auto const &node_child : *node) {
        string run_path = path + "/" + node_child->getName();

        TTestResult::TTest result;
        result.Path = run_path;

        if (node_child->getValue()) {
            try {
                LOGI << "RUN  " << run_path;
                node_child->getValue()->run();
                LOGI << "OK   " << run_path;
            } catch (Exception const &e) {
                result.Error = e.what();
            } catch (std::exception const &e) {
                result.Error = e.what();
            } catch (...) {
                result.Error = "unknown exception";
            }
            if (!result.Error.get().empty()) {
                LOGE << result.Error.get();
                LOGE << "FAIL " << run_path;
            }

            results.Test.add(result);
        } else
            run(results, node_child, path + "/" + node_child->getName());
    }
}


} // implementation
} // testing
} // iridium
