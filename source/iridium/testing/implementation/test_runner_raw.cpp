#include "test_runner_raw.h"

#include "iridium/testing/unit_test.h"
#include "iridium/logging/logger.h"
#include "iridium/parsing/implementation/json_parser.h"

#include <string>


using std::string;
using iridium::parsing::implementation::CJSONParser;


namespace iridium::testing::implementation {


TResult CTestRunnerRaw::run(INodeTest::TSharedPtr const &node_test) {
    TResult result;

    run(result, node_test);

    return result; // ----->
}


void CTestRunnerRaw::run(
    TResult                       &results,
    INodeTest::TSharedPtr   const &node,
    std::string             const &path)
{
    for (auto const &node_child : *node) {
        string run_path = path + "/" + node_child->getName();

        TResult::TTests tests;
        tests.Path = run_path;

        if (node_child->getValue()) {
            try {
                LOGI << "RUN  " << run_path;
                node_child->getValue()->run();
                LOGI << "OK   " << run_path;
            } catch (Exception const &e) {
                tests.Error = e.what();
            } catch (std::exception const &e) {
                tests.Error = e.what();
            } catch (...) {
                tests.Error = "unknown exception";
            }

            if (!tests.Error.get().empty()) {
                LOGE << "\n"    << tests.Error.get();
                LOGE << "FAIL " << run_path;
            }

            results.Tests.add(tests);
        } else
            run(results, node_child, path + "/" + node_child->getName());
    }
}


} // iridium::testing::implementation
