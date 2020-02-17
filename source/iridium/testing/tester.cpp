#include "tester.h"
#include "iridium/logging/logger.h"
#include "iridium/parsing/implementation/node_type.h"
#include "iridium/strings.h"
#include "iridium/assert.h"


using std::string;
using iridium::parsing::INodeType;
using iridium::parsing::implementation::CNodeType;


namespace iridium {
namespace testing {


typedef CNodeType<ITest *> CNodeTest;


string convertNodeTestToString(
    typename INodeTest::TConstSharedPtr const &node,
    string                              const &tab = "")
{
    if (!node)
        return "NULL"; // ----->

    string result = "\n" + tab + "'" + node->getName() + "'";

    for (auto const &i: *node)
        result += convertNodeTestToString(i, tab + "  ");

    return result; // ----->
}


//string const DEFAULT_TEST_ROOT_NODE_NAME = "root";


Tester::Tester() {
}


void Tester::add(ITest * const test, std::string const &path_) {
    m_test_list.push_back( std::pair<ITest *, std::string>(test, replace(path_, "\\", "/")));
}


int Tester::run(int argc, char* argv[], std::string const &main_cpp_path) {
    m_test_root_node = CNodeTest::create("root");
    logging::update(logging::config::createDefaultConsoleLoggerConfig());

    static string const DEFAULT_DELIMITER = "/";

    m_path_base = DEFAULT_DELIMITER + m_test_root_node->getName() + 
        DEFAULT_DELIMITER + replace(main_cpp_path, "\\", DEFAULT_DELIMITER);
    m_path_base = m_path_base.substr(0, m_path_base.find_last_of(DEFAULT_DELIMITER));

    for (auto const &item: m_test_list) {
        string path = DEFAULT_DELIMITER + m_test_root_node->getName() + DEFAULT_DELIMITER + item.second;
        ITest *test = item.first;

        if (path.substr(0, m_path_base.size()) == m_path_base)
            path = path.substr(m_path_base.size(), string::npos);

        if (m_test_root_node->findChilds(path).empty()) {
            auto node = m_test_root_node;
            for (auto const &node_name: split(path, DEFAULT_DELIMITER)) {
                auto next_node = node->getChild(node_name);
                if (next_node)
                    node = next_node;
                else
                    node = node->addChild(node_name);
            }
            node->setValue(test);
        } else {
            string  error = "test duplication: " + path;
            LOGF << error;
            throw std::runtime_error(error); // ----->
        }
    }

    if (argc == 2) {
        if (string(argv[1]) == "help"   ||
            string(argv[1]) == "--help" ||
            string(argv[1]) == "/?")
        {
            LOGI    << "\nusage:\n"
                    << std::string(argv[0]) << " help\n"
                    << std::string(argv[0]) << " list\n"
                    << std::string(argv[0]) << " run [ include_path ] [ exclude_path ]\n";
        }

        if (string(argv[1]) == "list")
            LOGI << "\ntests:" << m_test_root_node;
        return 0; // ----->
    }

    if (argc > 2 && string(argv[1]) == "run") {
        m_path_include = argv[2];
        if (argc == 4)
            m_path_exclude = argv[3];
    }

    runTests(m_test_root_node, "");

    LOGI << "\n\ntotal:  " << m_test_list.size() << "\nfailed: " << m_failed_paths.size();
    // todo: tested. passed

    if (!m_failed_paths.empty()) {
        string tests;
        for (auto const &line: m_failed_paths)
            tests += line + "\n\n";
        LOGE << "\n\nfailed tests:\n" + tests;
    }

    return m_failed_paths.empty(); // ----->
}


void Tester::runTests(INodeTest::TSharedPtr const &node, std::string const &path) {
    for (auto const &i : *node) {
        string run_path = path + "/" + i->getName();
        if (i->getValue() &&
            run_path.substr(0, m_path_include.size()) == m_path_include && (m_path_exclude.empty() ||
            run_path.substr(0, m_path_exclude.size()) != m_path_exclude))
        {
            try {
                LOGI << "RUN  " << run_path;
                i->getValue()->run();
                LOGI << "OK   " << run_path;
            } catch (Exception const &e) {
                string error = run_path + "\n" + e.what();
                m_failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            } catch (std::exception const &e) {
                string error = run_path + "\n" + e.what();
                m_failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            } catch (...) {
                string error = run_path + "\nunknown exception";
                m_failed_paths.push_back(error);
                LOGE << "FAIL " << error;
            }
        } else
            runTests(i, path + "/" + i->getName());
    }
}


} // testing
} // iridium


IMPLEMENT_CONVERT(std::string, iridium::testing::INodeTest::TConstSharedPtr , convertNodeTestToString)
IMPLEMENT_CONVERT(std::string, iridium::testing::INodeTest::TSharedPtr      , convertNodeTestToString)
