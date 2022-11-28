#include "test_runner_raw.h"

#include "iridium/testing/unit_test.h"
#include "iridium/logging/logger.h"

#include <string>


using std::string;


namespace iridium {
namespace testing {
namespace implementation {


CTestRunnerRaw::TResult CTestRunnerRaw::run(INodeTest::TSharedPtr const &node_test) {
    TResult result;
    run(result.passed, result.failed, node_test);
    return result;
}


void CTestRunnerRaw::run(
    std::list<std::string>        &passed_paths,
    std::list<std::string>        &failed_paths,
    INodeTest::TSharedPtr   const &node,
    std::string             const &path)
{
    for (auto const &node_child : *node) {
        string run_path = path + "/" + node_child->getName();
        if (node_child->getValue()) {
            try {
                LOGI << "RUN  " << run_path;
                node_child->getValue()->run();
                LOGI << "OK   " << run_path;
                passed_paths.push_back(run_path);
            } catch (Exception const &e) {
                string error = run_path + "\n" + e.what();
                failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            } catch (std::exception const &e) {
                string error = run_path + "\n" + e.what();
                failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            } catch (...) {
                string error = run_path + "\nunknown exception";
                failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            }
        } else
            run(passed_paths, failed_paths, node_child, path + "/" + node_child->getName());
    }
}


} // implementation
} // testing
} // iridium
