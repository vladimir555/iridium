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


class UnitTest {
public:
    UnitTest() = default;
   ~UnitTest() = default;

    template<typename TValue>
    void fail(
        TValue      const &left,
        TValue      const &right,
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
    void assert(TFunction const &func,
        std::string const &condition_source, std::string const &line);

    void assert(bool const &is_true,
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
    TValue      const &left,
    TValue      const &right,
    std::string const &condition_name,
    std::string const &condition_source,
    std::string const &line)
{
    // todo: print values
    throw Exception("'" + condition_source + "' at line " + line); // ----->
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
void UnitTest::assert(TFunction const &func,
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
