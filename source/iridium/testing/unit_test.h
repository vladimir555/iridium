// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
#define HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96


#include <string>
#include <stdexcept>

#include "iridium/convertion/convert.h"


namespace iridium {
namespace testing {


class Exception {
public:
    Exception(std::string const &what);
   ~Exception() = default;
    std::string what() const;
private:
    std::string m_what;
};

//template <class T, std::size_t = sizeof(T)>
//std::true_type  is_complete_impl(T *);
//std::false_type is_complete_impl(...);
//
//template <class T>
//using is_complete = decltype(is_complete_impl(std::declval<T*>()));
//
//template<typename TValue, is_complete<iridium::convertion::convert<std::string(TValue const &value)> >
//std::string convert(TValue const &value) {
//
//}

class UnitTest {
public:
    UnitTest() = default;
   ~UnitTest() = default;


//    template<typename TValue>
//    std::string convert(TValue value) {
//
//    }

    // todo: refactor fail template specifications based on convertion templates and based on (void *) specification

    template<typename TValue>
    void fail(
        TValue      const &left,
        TValue      const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);

    template<>
    void fail<std::string>(
        std::string const &left,
        std::string const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);

    template<>
    void fail<uint64_t>(
        uint64_t    const &left,
        uint64_t    const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);

    template<>
    void fail<int64_t>(
        int64_t     const &left,
        int64_t     const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);


    template<>
    void fail<double>(
        double      const &left,
        double      const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);

    template<>
    void fail<std::chrono::system_clock::time_point>(
        std::chrono::system_clock::time_point const &left,
        std::chrono::system_clock::time_point const &right,
        std::string const &condition_name,
        std::string const &condition_source,
        std::string const &line);

    void fail(
        std::string const &condition_source,
        std::string const &line);

    template<typename TLeft, typename TRight>
    void less(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TLeft, typename TRight>
    void lessEqual(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TLeft, typename TRight>
    void equal(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TLeft, typename TRight>
    void greater(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TLeft, typename TRight>
    void greaterEqual(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TFunction, typename TException>
    void assert_(TFunction const &func,
        std::string const &condition_source, std::string const &line);

    void assert_(bool const &is_true,
        std::string const &condition_source, std::string const &line);
};


// -----


template<>
void UnitTest::less(
    double const &left, double const &right,
    std::string const &condition_source, std::string const &line);


template<>
void UnitTest::lessEqual(double const &left, double const &right,
    std::string const &condition_source, std::string const &line);


template<>
void UnitTest::equal(double const &left, double const &right,
    std::string const &condition_source, std::string const &line);


template<>
void UnitTest::greater(double const &left, double const &right,
    std::string const &condition_source, std::string const &line);


template<>
void UnitTest::greaterEqual(double const &left, double const &right,
    std::string const &condition_source, std::string const &line);


template<typename TValue>
void UnitTest::fail(
    TValue      const &,
    TValue      const &,
    std::string const &,
    std::string const &condition_source,
    std::string const &line)
{
    throw Exception("'" + condition_source + "'\n" + line); // ----->
}


template<typename TLeft, typename TRight>
void UnitTest::less(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    TRight left_(left);
    if (!(left_ < right))
        fail(left_, right, "less", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTest::lessEqual(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    TRight left_(left);
    if (!(left_ <= right))
        fail(static_cast<TRight>(left), right, "lessEqual", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTest::equal(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    TRight left_ = static_cast<TRight>(left);
    if (!(left_ == right))
        fail(left_, right, "equal", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTest::greater(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    TRight left_(left);
    if (!(left_ > right))
        fail(left_, right, "greater", condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTest::greaterEqual(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    TRight left_(left);
    if (!(left_ >= right))
        fail(left_, right, "greaterEqual", condition_source, line);
}


template<typename TFunction, typename TException>
void UnitTest::assert_(TFunction const &func,
    std::string const &condition_source, std::string const &line)
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


} // testing
} // iridium


#endif // HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
