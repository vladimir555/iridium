// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "test_runner.h"


using std::string;
using iridium::testing::ITestRunner;


string convertNodeTestToString(
    typename ITestRunner::IUnitTestCaseNode const &node,
    string const &tab = "")
{
    string result = "\n" + tab + "'" + node.getName() + "'";

    for (auto const &i: node)
        result += convertNodeTestToString(*i, tab + "  ");

    return result; // ----->
}


IMPLEMENT_CONVERT(std::string, ITestRunner::IUnitTestCaseNode, convertNodeTestToString)
