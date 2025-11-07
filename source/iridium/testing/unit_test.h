// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96
#define HEADER_UNIT_TEST_DA33741E_8C9F_42D4_98D8_F6FCCCD87B96


#include <string>
#include <stdexcept>
#include <typeinfo>

#include "iridium/convertion/convert.h"


namespace iridium {
namespace testing {


/// \~english @brief A custom exception class for test failures.
/// \~russian @brief Пользовательский класс исключений для неудачных тестов.
class Exception {
public:
    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    Exception(std::string const &what);
    /// \~english @brief Destructor.
    /// \~russian @brief Деструктор.
   ~Exception() = default;
    /// \~english @brief Gets the exception message.
    /// \~russian @brief Возвращает сообщение об исключении.
    std::string what() const;
private:
    std::string m_what;
};

/// \~english @brief A base class for unit tests, providing assertion methods.
/// \~russian @brief Базовый класс для модульных тестов, предоставляющий методы утверждений.
class UnitTest {
public:
    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    UnitTest() = default;
    /// \~english @brief Destructor.
    /// \~russian @brief Деструктор.
   ~UnitTest() = default;

    /// \~english @brief Fails a test with a message.
    /// \~russian @brief "Проваливает" тест с сообщением.
    template<typename TValue>
    void fail(
        TValue      const &left,
        TValue      const &right,
        std::string const &,
        std::string const &condition_source,
        std::string const &line);

    /// \~english @brief Fails a test with a message.
    /// \~russian @brief "Проваливает" тест с сообщением.
    void fail(
        std::string const &condition_source,
        std::string const &line);

    /// \~english @brief Asserts that the left value is less than the right value.
    /// \~russian @brief Утверждает, что левое значение меньше правого.
    template<typename TLeft, typename TRight>
    void less(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that the left value is less than or equal to the right value.
    /// \~russian @brief Утверждает, что левое значение меньше или равно правому.
    template<typename TLeft, typename TRight>
    void lessEqual(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that the left value is equal to the right value.
    /// \~russian @brief Утверждает, что левое значение равно правому.
    template<typename TLeft, typename TRight>
    void equal(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that the left value is greater than the right value.
    /// \~russian @brief Утверждает, что левое значение больше правого.
    template<typename TLeft, typename TRight>
    void greater(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that the left value is greater than or equal to the right value.
    /// \~russian @brief Утверждает, что левое значение больше или равно правому.
    template<typename TLeft, typename TRight>
    void greaterEqual(TLeft const &left, TRight const &right,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that a function throws a specific exception.
    /// \~russian @brief Утверждает, что функция выбрасывает определенное исключение.
    template<typename TFunction, typename TException>
    void assert_(TFunction const &func,
        std::string const &condition_source, std::string const &line);

    /// \~english @brief Asserts that a condition is true.
    /// \~russian @brief Утверждает, что условие истинно.
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
    TValue      const &left,
    TValue      const &right,
    std::string const &,
    std::string const &condition_source,
    std::string const &line)
{
    std::string message;

    try {
        message = line
            +   "\n'" + condition_source + "'\n"
            +   "L: " + convertion::convert<std::string, TValue, true>(left)
            + "\nR: " + convertion::convert<std::string, TValue, true>(right);
    } catch (std::exception const &) {}

    if (message.empty()) {
        message = line
            + "\n'"         + condition_source
            +"'\ntype: "   + typeid(TValue).name()
            + "\nsize: "    + convertion::convert<std::string>(sizeof(TValue));
    }

    throw Exception(message);
}


template<>
void UnitTest::fail<std::string>(
    std::string const &left,
    std::string const &right,
    std::string const &,
    std::string const &condition_source,
    std::string const &line);


template<>
void UnitTest::fail<std::chrono::system_clock::time_point>(
    std::chrono::system_clock::time_point const &left,
    std::chrono::system_clock::time_point const &right,
    std::string const &,
    std::string const &condition_source,
    std::string const &line);


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
