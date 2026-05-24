// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
#define HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96


#include <string>
#include <stdexcept>
#include <typeinfo>

#include "iridium/convertion/convert.h"


namespace iridium::testing {


class Exception {
public:
    Exception(size_t const &line, std::string const &what);
   ~Exception() = default;
    size_t getLine() const;
    std::string what() const;
private:
    size_t
        m_line;
    std::string
        m_what;
};


class UnitTestCase {
public:
    UnitTestCase() = default;
   ~UnitTestCase() = default;

    template<typename TValue>
    void fail(
        TValue
            const &left,
        TValue
            const &right,
        std::string
            const &,
        std::string
            const &condition_source,
        size_t
            const &line);

    void fail(
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TLeft, typename TRight>
    void less(
        TLeft
            const &left,
        TRight
            const &right,
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TLeft, typename TRight>
    void lessEqual(
        TLeft
            const &left,
        TRight
            const &right,
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TLeft, typename TRight>
    void equal(
        TLeft
            const &left,
        TRight
            const &right,
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TLeft, typename TRight>
    void greater(
        TLeft
            const &left,
        TRight
            const &right,
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TLeft, typename TRight>
    void greaterEqual(
        TLeft
            const &left,
        TRight
            const &right,
        std::string
            const &condition_source,
        size_t
            const &line);

    template<typename TFunction, typename TException>
    void assert_(
        TFunction
            const &func,
        std::string
            const &condition_source,
        size_t
            const &line);

    void assert_(
        bool
            const &is_true,
        std::string
            const &condition_source,
        size_t
            const &line);
};


// -----


template<>
void UnitTestCase::less(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line);


template<>
void UnitTestCase::lessEqual(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line);


template<>
void UnitTestCase::equal(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line);


template<>
void UnitTestCase::greater(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line);


template<>
void UnitTestCase::greaterEqual(
    double
        const &left,
    double
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line);


template<typename TValue>
void UnitTestCase::fail(
    TValue
        const &left,
    TValue
        const &right,
    std::string
        const &,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    std::string message;

    try {
        message = "'" + condition_source + "'\n"
            +   "L: " + convertion::convert<std::string, TValue, true>(left)
            + "\nR: " + convertion::convert<std::string, TValue, true>(right);
    } catch (std::exception const &) {}

    if (message.empty()) {
        message = "'" + condition_source
            +"'\ntype: "    + typeid(TValue).name()
            + "\nsize: "    + convertion::convert<std::string>(sizeof(TValue));
    }

    throw Exception(line, message);
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
        const &line);


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
        const &line);


template<typename TLeft, typename TRight>
void UnitTestCase::less(
    TLeft
        const &left,
    TRight
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    TRight left_(left);
    if (!(left_ < right))
        fail(left_, right, "less", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTestCase::lessEqual(
    TLeft
        const &left,
    TRight
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    TRight left_(left);
    if (!(left_ <= right))
        fail(static_cast<TRight>(left), right, "lessEqual", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTestCase::equal(
    TLeft
        const &left,
    TRight
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    TRight left_ = static_cast<TRight>(left);
    if (!(left_ == right))
        fail(left_, right, "equal", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTestCase::greater(
    TLeft
        const &left,
    TRight
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    TRight left_(left);
    if (!(left_ > right))
        fail(left_, right, "greater", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTestCase::greaterEqual(
    TLeft
        const &left,
    TRight
        const &right,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    TRight left_(left);
    if (!(left_ >= right))
        fail(left_, right, "greaterEqual", condition_source, line);
}


template<typename TFunction, typename TException>
void UnitTestCase::assert_(
    TFunction
        const &func,
    std::string
        const &condition_source,
    size_t
        const &line)
{
    bool is_fail    = false;

    try {
        func();
        is_fail     = true;
        fail(condition_source, line);
    } catch (TException const &) {
    } catch (...) {
        is_fail     = true;
    }

    if (is_fail)
        fail(condition_source, line);
}


} // namespace iridium::testing


#endif // HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
