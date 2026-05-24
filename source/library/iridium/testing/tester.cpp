#include "tester.h"

#include "iridium/logging/logger.h"
#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/enum.h"

#include "iridium/parsing/implementation/json_parser.h"

#include "implementation/test_runner_raw.h"
#include "implementation/test_runner_fork.h"

// #include <unistd.h>


using std::string;
using std::vector;
using std::list;
using std::chrono::seconds;

using iridium::parsing::INodeType;
using iridium::parsing::implementation::CNodeType;
using iridium::parsing::implementation::CJSONParser;
using iridium::testing::implementation::CTestRunnerRaw;
using iridium::testing::implementation::CTestRunnerFork;
using iridium::convertion::convert;


namespace iridium::testing {


struct TCmdArgs {
    DEFINE_CREATE(TCmdArgs)

    DEFINE_ENUM(
        TCommand,
        HELP,
        RUN,
        LIST
    );

    DEFINE_ENUM(
        TMode,
        PARALLEL,
        SERIAL,
        RAW
    );

    DEFINE_ENUM(
        TPrintResult,
        JSON,
        CTEST_VSCODE
    );

    TCommand
        command;
    TMode
        mode;
    TPrintResult
        print_result;
    std::chrono::system_clock::duration
        timeout;
    string
        app_name;
    string
        include_path;
    list<string>
        exclude_paths;
};


} // namespace iridium::testing


//DEFINE_ENUM_CONVERT(iridium::testing::TCmdArgs::TCommand);
//DEFINE_ENUM_CONVERT(iridium::testing::TCmdArgs::TMode);
//DEFINE_ENUM_CONVERT(iridium::testing::TCmdArgs::TPrintResult);

IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TCommand);
IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TMode);
IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TPrintResult);


namespace iridium::testing {


typedef CNodeType<IUnitTestCase *>
    CNodeTest;


void Tester::add(
    IUnitTestCase
      * const  test,
    std::string
        const &path_,
    size_t
        const &line,
    std::string
        const &name)
{
    auto path = path_;
    std::replace(path.begin(), path.end(), '\\', '/');
    TEntry entry {
        path,
        line,
        name
    };
    m_map_path_test[entry] = test;
}


template<typename TCmdArgEnum>
string convertCmdArgEnumsToString() {
    string result;

    for (auto const &enum_: TCmdArgEnum::getEnums())
        result += convert<string>(enum_) + "|";

    if(!result.empty())
        result.pop_back();

    return lowerCase(replace(result, "_", "-")); // ----->
}


TCmdArgs::TSharedPtr parseCommandLine(int argc, char* argv[]) {
    vector<string> args;

    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    try {
        TCmdArgs::TSharedPtr result = TCmdArgs::create(
            // not compartible with visual studio
            //TCmdArgs {
            //    .command            = TCmdArgs::TCommand::RUN,
            //    .mode               = TCmdArgs::TMode::PARALLEL,
            //    .print_result       = TCmdArgs::TPrintResult::UNKNOWN,
            //    .timeout            = std::chrono::minutes(1),
            //    .app_name           = args[0],
            //    .include_path       = "/"
            //}
            TCmdArgs {
                TCmdArgs::TCommand::RUN,
                TCmdArgs::TMode::RAW,
                TCmdArgs::TPrintResult::UNKNOWN,
                std::chrono::minutes(1),
                args[0],
                "/",
                {}
            }
        );

        result->app_name = argv[0];

        if (args.size() == 1)
            return result; // ----->

        if (args.size() >  1)
            result->command = convert<TCmdArgs::TCommand>(args[1]);

        if (result->command == TCmdArgs::TCommand::HELP)
            throw nullptr; // --->

        if (result->command == TCmdArgs::TCommand::LIST) {
            if (args.size() == 2)
                return result; // ----->
            else
                throw nullptr; // --->
        }

        if (args.size() >= 2 && args[1] == "run") {
            size_t i = 2;

            static string const TIMEOUT         = "--timeout";
            static string const PRINT_RESULT    = "--print-result";
            static string const MODE            = "--mode";

            while (i < args.size() && args[i].substr(0, 2) == "--") {
                string arg_name;
                string arg_value;

                {
                    auto arg_entry = split(args[i], "=");
                    arg_name = arg_entry.front();
                    if (arg_entry.size() == 2)
                        arg_value = replace(arg_entry.back(), "-", "_");
                }

                if (arg_name == MODE)
                    result->mode = convert<TCmdArgs::TMode>(arg_value);
                else
                if (arg_name == PRINT_RESULT)
                    result->print_result = convert<TCmdArgs::TPrintResult>(arg_value);
                else
                if (arg_name == TIMEOUT)
                    result->timeout = convert<std::chrono::system_clock::duration>(arg_value);
                else
                    throw nullptr; // --->

                i++;
            }

            // LOGT << result->timeout;

            if (args.size() > 2 && i < args.size()) {
                result->include_path = args[i];
                i++;
            }

            if (args.size() > 3) {
                for (; i < args.size(); i++)
                    result->exclude_paths.push_back(args[i]);
            }

            return result; // ----->
        } else
            throw nullptr; // --->

        // return result; // ----->
    } catch (std::exception const &e) {
        LOGE << e.what();
    } catch (...) {
    }

    LOGI << "\nusage:\n"
        << args[0] << " help\n"
        << args[0] << " list\n"
        << args[0] << " run [ --mode="
        << convertCmdArgEnumsToString<TCmdArgs::TMode>()
        << " ] [ --print-result="
        << convertCmdArgEnumsToString<TCmdArgs::TPrintResult>()
        << " ] [ --timeout=seconds ] [ include_path ] [ exclude_path ] ... [ exclude_path ]\n"
        << "example:\n"
        << args[0] << " run / \n"
        << args[0] << " run --timeout=12345 --mode=serial / \n"
        << args[0] << " run --timeout=5m50s --mode=parallel / \n";

    return nullptr; // ----->
}


int Tester::run(int argc, char* argv[], std::string const &main_cpp_path) {
    {
        auto config = logging::config::createDefault();
        logging::setConfig(config);
    }

    try {
        string testing_source_directory_path = main_cpp_path;

        std::replace(testing_source_directory_path.begin(), testing_source_directory_path.end(), '\\', '/');
        testing_source_directory_path = testing_source_directory_path.substr(0, testing_source_directory_path.find_last_of('/'));

        auto args = parseCommandLine(argc, argv);

        if (args) {
//            {
//                using iridium::logging::config::TLogger;
//                TLogger config = logging::Logger::instance().getConfig();
//                TLogger::TSink file_sink;
//
//                file_sink.Url = args->app_name + ".log";
//                file_sink.Type = TLogger::TSink::TSinkType::FILE;
//                file_sink.IsAsync = true;
//                config.Sink.add(file_sink);
//
//                iridium::logging::setConfig(config);
//            }

//            LOGT << "\napp          : " << args->app_name
//                 << "\ncommand      : " << args->command
//                 << "\nmode         : " << args->mode
//                 << "\ntimeout      : " << args->timeout
//                 << "\nprint_result : " << args->print_result
//                 << "\ninclude_path : " << args->include_path;

            if (args->command == TCmdArgs::TCommand::LIST) {
//                LOGI << getTestTree(main_cpp_path);

                // auto main_cpp_path_ = main_cpp_path;
                // std::replace(main_cpp_path_.begin(), main_cpp_path_.end(), '\\', '/');
                // main_cpp_path_ = main_cpp_path_.substr(0, main_cpp_path_.find_last_of('/'));

                string paths;
                for (auto const &path_test: m_map_path_test)
                    paths += "\n"
                        + path_test.first.path.substr(testing_source_directory_path.size()) + "/"
                        + path_test.first.name;
                LOGI << "\n" << paths << "\n\ntotal:  " << m_map_path_test.size();
                return 0; // ----->
            }

            ITestRunner::TSharedPtr test_runner;

            if (args->mode == TCmdArgs::TMode::RAW) {
                // using namespace iridium::logging::config;

                // auto config = createDefault();

                // TLogger::TSink file_sink;
                // file_sink.Uri       = convert<string>(::getpid()) + ".log";
                // file_sink.Type      = TLogger::TSink::TSinkType::FILE;
                // // file_sink.Level     = iridium::logging::TEvent::TLevel::TRACE;
                // file_sink.IsAsync   = true;
                // config.Sink.add(file_sink);

                // iridium::logging::setConfig(config);

                test_runner = CTestRunnerRaw::create();
            } else {
                test_runner = CTestRunnerFork::create
                    (args->app_name, args->timeout, args->mode == TCmdArgs::TMode::SERIAL);
            }

            auto root   = getTestTree(testing_source_directory_path, args->include_path, args->exclude_paths);
            auto result = test_runner->run(root);

            size_t failed_count = 0;
            size_t passed_count = 0;
            string errors;

            for (auto const &test_case: result.TestCases) {
                if (test_case.Error.get().empty()) {
                    passed_count++;
                } else {
                    size_t line = test_case.Line.get();
                    errors += "\n" +
                        test_case.Path.get()  + ":"  +
                        convert<string>(line) + "/"  +
                        test_case.Name.get()  + "\n" +
                        test_case.Error.get() + "\n";
                    failed_count++;
                }
            }

            if (args->print_result == TCmdArgs::TPrintResult::UNKNOWN) {
                if (!errors.empty())
                    LOGE << "\nERRORS:\n" << errors;

                LOGI << "\npassed: " << passed_count
                     << "\nfailed: " << failed_count
                     << "\ntotal:  " << m_map_path_test.size();
            }

            if (args->print_result == TCmdArgs::TPrintResult::JSON) {
                auto json = CJSONParser::create()->compose(result.getNode());
                LOGI << "\n\n" << json << "\n" << json.size();
            }

            // for vscode ctest stdout parsing
            if(args->print_result == TCmdArgs::TPrintResult::CTEST_VSCODE) {
                for (auto test_case: result.TestCases) {
                    if (!test_case.Error.get().empty()) {
                        std::cerr
                            << testing_source_directory_path
                            << test_case.Path.get() << ":"
                            << test_case.Line.get()
                            << ": error: "
                            << replace(test_case.Error.get(), "\n", "; ")
                            << std::endl;
                    }
                }
            }

            return !errors.empty(); // ----->
        } else
            return 1; // ----->
    } catch (std::exception const &e) {
        LOGF << e.what();
        return 1;
    } catch (char const *e) {
        LOGF << e;
        return 1;
    } catch (...) {
        LOGF << "unknown exception";
        return 1;
    }
    // return 0;
}


Tester::IUnitTestCaseNode::TSharedPtr Tester::getTestTree(
    string
        const &testing_source_directory_path,
    string
        const &include,
    list<string>
        const &excludes) const
{
    static string const ROOT_NODE_NAME = "root";

    IUnitTestCaseNode::TSharedPtr root_node = CNodeTest::create(ROOT_NODE_NAME);

    for (auto const &path_test: m_map_path_test) {

        auto path = path_test.first.path + "/" + path_test.first.name;
        if (path.substr(0, testing_source_directory_path.size()) == testing_source_directory_path)
            path = path.substr(testing_source_directory_path.size());

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


} // iridium::testing
