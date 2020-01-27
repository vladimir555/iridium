#ifndef HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
#define HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96


#include <string>
#include <stdexcept>

#include "iridium/logging/logger.h"

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

    void fail(std::string const &condition_source, std::string const &line);

    template<typename TValue>
    void less(TValue const &left, TValue const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TValue>
    void lessEqual(TValue const &left, TValue const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TValue>
    void equal(TValue const &left, TValue const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TValue>
    void greater(TValue const &left, TValue const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TValue>
    void greaterEqual(TValue const &left, TValue const &right,
        std::string const &condition_source, std::string const &line);

    template<typename TFunction, typename TException>
    void assert(TFunction const &func,
        std::string const &condition_source, std::string const &line);

    void assert(bool const &is_true,
        std::string const &condition_source, std::string const &line);

    template<typename TLeft, typename TRight>
    void equal(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);
};


// -----


template<typename TValue>
void UnitTest::less(TValue const &left, TValue const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(left < right))
        fail(condition_source, line);
}


template<typename TValue>
void UnitTest::lessEqual(TValue const &left, TValue const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(left <= right))
        fail(condition_source, line);
}


template<typename TValue>
void UnitTest::equal(TValue const &left, TValue const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(left == right))
        fail(condition_source, line);
}


template<typename TValue>
void UnitTest::greater(TValue const &left, TValue const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(left > right))
        fail(condition_source, line);
}


template<typename TValue>
void UnitTest::greaterEqual(TValue const &left, TValue const &right,
    std::string const &condition_source, std::string const &line)
{
    if (!(left >= right))
        fail(condition_source, line);
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
    } catch (TException const &e) {
    } catch (...) {
        is_fail     = true;
    }

    if (is_fail)
        fail(condition_source, line);
}


template<typename TLeft, typename TRight>
void UnitTest::equal(TLeft const &left, TRight const &right,
    std::string const &condition_source, std::string const &line)
{
    equal<TRight>(left, right, condition_source, line);
}


} // testing
} // iridium


#endif // HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
