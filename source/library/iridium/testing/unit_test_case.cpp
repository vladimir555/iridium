// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "unit_test_case.h"

#include <cmath>


namespace iridium::testing {


Exception::Exception(size_t const &line, std::string const &what)
:
    m_line(line),
    m_what(what)
{}


size_t Exception::getLine() const {
    return m_line; // ----->
}


std::string Exception::what() const {
    return m_what; // ----->
}


void UnitTestCase::fail(
    std::string
        const &condition_source,
    size_t
        const &line)
{
    throw Exception(line, "'" + condition_source); // ----->
}


template<>
void UnitTestCase::fail<std::string>(
    std::string
        const &left,
    std::string
        const &right,
    std::string
        const &,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    throw Exception(line,
          "\n'"    + condition_source
        + "\nL: '" + left
        +"'\nR: '" + right + "'"); // ----->
}


template<>
void UnitTestCase::fail<std::chrono::system_clock::time_point>(
    std::chrono::system_clock::time_point
        const &left,
    std::chrono::system_clock::time_point
        const &right,
    std::string
        const &,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    throw Exception(line,
          "\n'"    + condition_source
        + "\nL: '" + convertion::convert<std::string>(left)
        + "\nR: '" + convertion::convert<std::string>(right)); // ----->
}


template<>
void UnitTestCase::less(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!std::isless(left, right))
        fail(left, right, "less", condition_source, line);
}


template<>
void UnitTestCase::lessEqual(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!std::islessequal(left, right))
        fail(left, right, "lessEqual", condition_source, line);
}


template<>
void UnitTestCase::equal(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!(std::fabs(left - right) < std::numeric_limits<double>::epsilon()))
        fail(left, right, "equal", condition_source, line);
}


template<>
void UnitTestCase::greater(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!std::isgreater(left, right))
        fail(left, right, "greater", condition_source, line);
}


template<>
void UnitTestCase::greaterEqual(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!std::isgreaterequal(left, right))
        fail(left, right, "lessEqual", condition_source, line);
}


void UnitTestCase::assert_(
    bool
        const &is_true,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    if (!is_true)
        fail(condition_source, line);
}


} // namespace iridium::testing
