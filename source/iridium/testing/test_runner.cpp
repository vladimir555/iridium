#include "test_runner.h"


using std::string;
using iridium::testing::ITestRunner;


string convertNodeTestToString(
    typename ITestRunner::INodeTest const &node,
    string const &tab = "")
{
    string result = "\n" + tab + "'" + node.getName() + "'";

    for (auto const &i: node)
        result += convertNodeTestToString(*i, tab + "  ");

    return result; // ----->
}


IMPLEMENT_CONVERT(std::string, ITestRunner::INodeTest, convertNodeTestToString)
