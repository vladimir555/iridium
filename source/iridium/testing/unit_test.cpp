#include "unit_test.h"

#include <cmath>


// todo: long double
// todo: print values


namespace iridium {
namespace testing {


Exception::Exception(std::string const &what)
:
    m_what(what)
{}


std::string Exception::what() const {
    return m_what; // ----->
}


void UnitTest::fail(
    std::string const &condition_source,
    std::string const &line)
{
    throw Exception("'" + condition_source + "' at line " + line); // ----->
}


template<>
void UnitTest::less(double const &left, double const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!std::isless(left, right))
        fail(left, right, "less", condition_source, line);
}


template<>
void UnitTest::lessEqual(double const &left, double const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!std::islessequal(left, right))
        fail(left, right, "lessEqual", condition_source, line);
}


template<>
void UnitTest::equal(double const &left, double const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(std::fabs(left - right) < std::numeric_limits<double>::epsilon()))
        fail(left, right, "equal", condition_source, line);
}


template<>
void UnitTest::greater(double const &left, double const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!std::isgreater(left, right))
        fail(left, right, "greater", condition_source, line);
}


template<>
void UnitTest::greaterEqual(double const &left, double const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!std::isgreaterequal(left, right))
        fail(left, right, "lessEqual", condition_source, line);
}


void UnitTest::assert_(bool const &is_true,
    std::string const &condition_source, std::string const &line)
{
    if (!is_true)
        fail(condition_source, line);
}


} // testing
} // iridium
