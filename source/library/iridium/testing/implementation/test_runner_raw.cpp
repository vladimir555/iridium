// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "test_runner_raw.h"

#include "iridium/testing/unit_test_case.h"
#include "iridium/logging/logger.h"
#include "iridium/parsing/implementation/json_parser.h"

#include <string>


using std::string;
using iridium::parsing::implementation::CJSONParser;


namespace iridium::testing::implementation {


TTestRunResult CTestRunnerRaw::run(IUnitTestCaseNode::TSharedPtr const &node_test) {
    TTestRunResult test_run_result;

    run(test_run_result, node_test);

    return test_run_result; // ----->
}


void CTestRunnerRaw::run(
    TTestRunResult
              &test_run_result,
    IUnitTestCaseNode::TSharedPtr
        const &unit_test_case_tree,
    std::string
        const &path)
{
    for (auto const &test_case_node: *unit_test_case_tree) {
        auto run_path = path + "/" + test_case_node->getName();

        TTestRunResult::TTestCases test_case;
        test_case.Path = path;
        test_case.Name = test_case_node->getName();

        if (test_case_node->getValue()) {
            try {
                LOGI << "RUN  " << run_path;
                test_case_node->getValue()->run();
                LOGI << "OK   " << run_path;
            } catch (Exception const &e) {
                test_case.Line  = e.getLine();
                test_case.Error = e.what();
            } catch (std::exception const &e) {
                test_case.Line  = test_case_node->getValue()->getLine();
                test_case.Error = string("exception: ") + e.what();
            } catch (...) {
                test_case.Error = "unknown exception";
            }

            if (!test_case.Error.get().empty()) {
                LOGE << "\n"    << test_case.Error.get();
                LOGE << "FAIL " << run_path;
            }

            test_run_result.TestCases.add(test_case);
        } else
            run(test_run_result, test_case_node, path + "/" + test_case_node->getName());
    }
}


} // iridium::testing::implementation
