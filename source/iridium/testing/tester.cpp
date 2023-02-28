#include "tester.h"
#include "iridium/logging/logger.h"
#include "iridium/parsing/implementation/node_type.h"
#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/items.h"


#include "unit_test.h"

#include "iridium/testing/implementation/test_runner_raw.h"
#include "iridium/testing/implementation/test_runner_fork.h"
#include "iridium/parsing/implementation/parser_json.h"


using std::string;
using std::vector;
using std::list;
using iridium::parsing::INodeType;
using iridium::parsing::implementation::CNodeType;
using iridium::testing::implementation::CTestRunnerRaw;
using iridium::testing::implementation::CTestRunnerFork;

using iridium::parsing::implementation::CJSONParser;


namespace iridium {
namespace testing {


typedef CNodeType<ITest *> CNodeTest;


void Tester::add(ITest * const test, std::string const &path_) {
    auto path = path_;
    std::replace(path.begin(), path.end(), '\\', '/');
    m_map_path_test[path] = test;
}


int Tester::run(int argc, char* argv[], std::string const &main_cpp_path) {
    logging::update(logging::config::createDefaultConsoleLoggerConfig());

    vector<string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    if (args.size() == 2 && args[1] == "list") {
        LOGI << getTestTree(main_cpp_path);
        return 0; // ----->
    }

    if (args.size() == 1 ||  (args.size() > 1 && args[1] == "run")) {
        size_t  i = 2;
        bool    is_raw = args.size() > i && args[i] == "--raw";

        if (is_raw)
            i++;

        string  include = args.size() > i ? args[i] : "/";

        list<string> excludes;
        for (++i; i < args.size(); i++)
            excludes.push_back(args[i]);

        auto root = getTestTree(main_cpp_path, include, excludes);

        ITestRunner::TSharedPtr test_runner;

        // todo: args
        std::chrono::milliseconds timeout(5000);

        if (is_raw)
            test_runner = CTestRunnerRaw::create();
        else
            test_runner = CTestRunnerFork::create(args[0], timeout);

        auto result = test_runner->run(root);

        size_t failed_count = 0;
        size_t passed_count = 0;

        if (is_raw) {
            auto json = CJSONParser::create()->compose(result.getNode());
            LOGI << "\n\n" << json << "\n" << json.size();
        } else {
            string failed_tests;
            for (auto const &line: result.Test) {
                if (!line.Error.get().empty()) {
                    failed_count++;
                    failed_tests +=
                        line.Path.get()  + "\n" +
                        line.Error.get() + "\n\n";
                } else {
                    passed_count++;
                }
            }

            if (!failed_tests.empty())
                LOGE << "\n\nfailed tests:\n" + failed_tests;

            LOGI << "\npassed: " << passed_count
                 << "\nfailed: " << failed_count
                 << "\ntotal:  " << m_map_path_test.size();

        }
        return failed_count == 0; // ----->
    } else {
        LOGI << "\nusage:\n"
            << args[0] << " help\n"
            << args[0] << " list\n"
            << args[0] << " run [--raw] [ include_path ] [ exclude_path ] ... [ exclude_path ]\n";
        return 0; // ----->
    }
}


Tester::INodeTest::TSharedPtr Tester::getTestTree(
    string          const &main_cpp_path_,
    string          const &include,
    list<string>    const &excludes
) const {

    static string   const ROOT_NODE_NAME = "root";

    auto main_cpp_path = main_cpp_path_;
    std::replace(main_cpp_path.begin(), main_cpp_path.end(), '\\', '/');
    main_cpp_path = main_cpp_path.substr(0, main_cpp_path.find_last_of('/'));

    INodeTest::TSharedPtr root_node = CNodeTest::create(ROOT_NODE_NAME);
    for (auto const &path_test: m_map_path_test) {

        auto path = path_test.first;
        if (path.substr(0, main_cpp_path.size()) == main_cpp_path)
            path = path.substr(main_cpp_path.size());

        if (path.substr(0, include.size()) != include)
            continue; // <---

        bool is_excluded = false;
        for (auto const &exclude: excludes) {
            if (path.substr(0, exclude.size()) == exclude) {
                is_excluded = true; // <---
                break;
            }
        }

        if (is_excluded)
            continue; // <---

        auto node = root_node;
        for (auto const &name: split(path, "/")) {
            auto child = node->getChild(name);
            if (child)
                node = child;
            else
                node = node->addChild(name);
        }
        node->setValue(path_test.second);
    }
    return root_node;
}


} // testing
} // iridium
