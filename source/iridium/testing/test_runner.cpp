#include "test_runner.h"


using std::string;
using iridium::testing::ITestRunner;


string convertNodeTestToString(
    typename ITestRunner::INodeTest::TConstSharedPtr const &node,
    string const &tab = "")
{
    if (!node)
        return "NULL"; // ----->

    string result = "\n" + tab + "'" + node->getName() + "'";

    for (auto const &i: *node)
        result += convertNodeTestToString(i, tab + "  ");

    return result; // ----->
}


IMPLEMENT_CONVERT(std::string, ITestRunner::INodeTest::TConstSharedPtr , convertNodeTestToString)
IMPLEMENT_CONVERT(std::string, ITestRunner::INodeTest::TSharedPtr      , convertNodeTestToString)
