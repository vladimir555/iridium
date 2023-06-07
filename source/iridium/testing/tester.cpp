#include "tester.h"
#include "unit_test.h"

#include "iridium/logging/logger.h"
#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/items.h"

#include "iridium/parsing/implementation/parser_json.h"

#include "implementation/test_runner_raw.h"
#include "implementation/test_runner_fork.h"


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


namespace iridium {
namespace testing {


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
        JSON
    );

    TCommand                command;
    TMode                   mode;
    TPrintResult            print_result;
    std::chrono::seconds    timeout;
    string                  app_name;
    string                  include_path;
    list<string>            exclude_paths;
};


} // testing
} // iridium


IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TCommand);
IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TMode);
IMPLEMENT_ENUM(iridium::testing::TCmdArgs::TPrintResult);


namespace iridium {
namespace testing {


typedef CNodeType<ITest *> CNodeTest;


void Tester::add(ITest * const test, std::string const &path_) {
    auto path = path_;
    std::replace(path.begin(), path.end(), '\\', '/');
    m_map_path_test[path] = test;
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
                TCmdArgs::TMode::PARALLEL,
                TCmdArgs::TPrintResult::UNKNOWN,
                std::chrono::minutes(1),
                args[0],
                "/"
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
                if (args[i].substr(0, MODE.size()) == MODE)
                    result->mode = convert<TCmdArgs::TMode>(split(args[i], "=").back());
                else
                if (args[i].substr(0, PRINT_RESULT.size()) == PRINT_RESULT)
                    result->print_result = convert<TCmdArgs::TPrintResult>(split(args[i], "=").back());
                else
                if (args[i].substr(0, TIMEOUT.size()) == TIMEOUT)
                    result->timeout = seconds(convert<uint64_t>(split(args[i], "=").back()));
                else
                    throw nullptr; // --->
                i++;
            }

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

        return result; // ----->
    } catch (std::exception const &e) {
        LOGE << e.what();
    } catch (...) {
    }

    // todo: print availabe enums
    LOGI << "\nusage:\n"
        << args[0] << " help\n"
        << args[0] << " list\n"
        << args[0] << " run [ --mode=raw|serial|parallel ] [ --print-result=json ]"
        << " [ --timeout=seconds ] [ include_path ] [ exclude_path ] ... [ exclude_path ]\n";

    return nullptr; // ----->
}


int Tester::run(int argc, char* argv[], std::string const &main_cpp_path) {
    {
        auto config = logging::config::createDefault();
        config.Sink.begin()->IsAsync = true;
        logging::setConfig(config);
    }
    
    try {
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
                LOGI << getTestTree(main_cpp_path);
                return 0; // ----->
            }

            ITestRunner::TSharedPtr test_runner;

            if (args->mode == TCmdArgs::TMode::RAW)
                test_runner = CTestRunnerRaw::create();
            else
                test_runner = CTestRunnerFork::create(args->app_name, args->timeout);

            auto root   = getTestTree(main_cpp_path, args->include_path, args->exclude_paths);
            auto result = test_runner->run(root);

            size_t failed_count = 0;
            size_t passed_count = 0;
            string errors;
            for (auto const &test: result.Tests) {
                if (test.Error.get().empty())
                    passed_count++;
                else {
    //                if (test.Output.get().empty())
                    errors += "\n" + test.Path.get() + "\n" + test.Error.get() + "\n";
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

            return !errors.empty(); // ----->
        } else
            return 1; // ----->
    } catch (std::exception const &e) {
        LOGF << e.what();
        return 1;
    }
    return 0;
}


Tester::INodeTest::TSharedPtr Tester::getTestTree(
    string          const &main_cpp_path_,
    string          const &include,
    list<string>    const &excludes) const
{
    static string const ROOT_NODE_NAME = "root";

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
