// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
/// \~english @brief This macro defines the purpose of HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED.
///     English detailed description (if any).
/// \~russian @brief Этот макрос defines the purpose of HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED.
///     Russian detailed description (if any).
#define HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED


#include <stdexcept>
#include <string>

#include "iridium/convertion/convert.h"


namespace iridium {


template<typename T, typename TException>
/// \~english @brief Checks if a value exists (is not false, null, or empty) and throws a custom exception if it doesn't.
/// \~russian @brief Проверяет существование значения (не false, не null, не пустое) и выбрасывает пользовательское исключение, если оно не существует.
/// \~english @tparam T The type of the value to check.
/// \~russian @tparam T Тип проверяемого значения.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param value The value to check. It's a universal reference, so it can be an lvalue or an rvalue.
/// \~russian @param value Проверяемое значение. Это универсальная ссылка, поэтому это может быть lvalue или rvalue.
/// \~english @param exception The exception to throw if the value does not exist.
/// \~russian @param exception Исключение, которое будет выброшено, если значение не существует.
/// \~english @return The input value if it exists, otherwise throws an exception.
/// \~russian @return Входное значение, если оно существует, в противном случае выбрасывает исключение.
T &&assertExists(T &&value, TException const &exception) {
    if (static_cast<bool>(value))
        return std::forward<T>(value); // ----->
    else
        throw exception; // ----->
}


template<typename T>
/// \~english @brief Checks if a value exists (is not false, null, or empty) and throws a std::runtime_error if it doesn't.
/// \~russian @brief Проверяет существование значения (не false, не null, не пустое) и выбрасывает std::runtime_error, если оно не существует.
/// \~english @tparam T The type of the value to check.
/// \~russian @tparam T Тип проверяемого значения.
/// \~english @param value The value to check. It's a universal reference, so it can be an lvalue or an rvalue.
/// \~russian @param value Проверяемое значение. Это универсальная ссылка, поэтому это может быть lvalue или rvalue.
/// \~english @param error The error message for the std::runtime_error.
/// \~russian @param error Сообщение об ошибке для std::runtime_error.
/// \~english @return The input value if it exists, otherwise throws a std::runtime_error.
/// \~russian @return Входное значение, если оно существует, в противном случае выбрасывает std::runtime_error.
T &&assertExists(T &&value, std::string const &error) {
    return assertExists(std::forward<T>(value), std::runtime_error(error)); // ----->
}


template<typename T, typename TException>
/// \~english @brief Checks if a pointer is not null and throws a custom exception if it is.
/// \~russian @brief Проверяет, что указатель не является нулевым, и выбрасывает пользовательское исключение, если он нулевой.
/// \~english @tparam T The type of the pointed-to value.
/// \~russian @tparam T Тип значения, на которое указывает указатель.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param value The pointer to check.
/// \~russian @param value Проверяемый указатель.
/// \~english @param exception The exception to throw if the pointer is null.
/// \~russian @param exception Исключение, которое будет выброшено, если указатель нулевой.
/// \~english @return The input pointer if it's not null, otherwise throws an exception.
/// \~russian @return Входной указатель, если он не нулевой, в противном случае выбрасывает исключение.
T *assertExists(T *value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


template<typename T>
/// \~english @brief Checks if a pointer is not null and throws a std::runtime_error if it is.
/// \~russian @brief Проверяет, что указатель не является нулевым, и выбрасывает std::runtime_error, если он нулевой.
/// \~english @tparam T The type of the pointed-to value.
/// \~russian @tparam T Тип значения, на которое указывает указатель.
/// \~english @param value The pointer to check.
/// \~russian @param value Проверяемый указатель.
/// \~english @param error The error message for the std::runtime_error.
/// \~russian @param error Сообщение об ошибке для std::runtime_error.
/// \~english @return The input pointer if it's not null, otherwise throws a std::runtime_error.
/// \~russian @return Входной указатель, если он не нулевой, в противном случае выбрасывает std::runtime_error.
T *assertExists(T *value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


template<typename T, typename TException>
/// \~english @brief Checks if a std::shared_ptr is not null and throws a custom exception if it is.
/// \~russian @brief Проверяет, что std::shared_ptr не является нулевым, и выбрасывает пользовательское исключение, если он нулевой.
/// \~english @tparam T The type of the pointed-to value by std::shared_ptr.
/// \~russian @tparam T Тип значения, на которое указывает std::shared_ptr.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param value The std::shared_ptr to check.
/// \~russian @param value Проверяемый std::shared_ptr.
/// \~english @param exception The exception to throw if the std::shared_ptr is null.
/// \~russian @param exception Исключение, которое будет выброшено, если std::shared_ptr нулевой.
/// \~english @return The input std::shared_ptr if it's not null, otherwise throws an exception.
/// \~russian @return Входной std::shared_ptr, если он не нулевой, в противном случае выбрасывает исключение.
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


template<typename T>
/// \~english @brief Checks if a std::shared_ptr is not null and throws a std::runtime_error if it is.
/// \~russian @brief Проверяет, что std::shared_ptr не является нулевым, и выбрасывает std::runtime_error, если он нулевой.
/// \~english @tparam T The type of the pointed-to value by std::shared_ptr.
/// \~russian @tparam T Тип значения, на которое указывает std::shared_ptr.
/// \~english @param value The std::shared_ptr to check.
/// \~russian @param value Проверяемый std::shared_ptr.
/// \~english @param error The error message for the std::runtime_error.
/// \~russian @param error Сообщение об ошибке для std::runtime_error.
/// \~english @return The input std::shared_ptr if it's not null, otherwise throws a std::runtime_error.
/// \~russian @return Входной std::shared_ptr, если он не нулевой, в противном случае выбрасывает std::runtime_error.
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


template<typename T, typename = void>
/// \~english @brief Type trait to check if a container type T has a size() method. Default is std::false_type.
/// \~russian @brief Метафункция для проверки наличия у контейнерного типа T метода size(). По умолчанию std::false_type.
/// \~english @tparam T The container type to check.
/// \~russian @tparam T Проверяемый контейнерный тип.
struct TContainerHasSizeMethod: std::false_type {};


template<typename T>
/// \~english @brief Type trait to check if a container type T has a size() method. Specialization for types that have size().
/// \~russian @brief Метафункция для проверки наличия у контейнерного типа T метода size(). Специализация для типов, имеющих size().
/// \~english @tparam T The container type to check.
/// \~russian @tparam T Проверяемый контейнерный тип.
struct TContainerHasSizeMethod<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};


template<typename T, typename TException>
/// \~english @brief This function computes assertSize.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertSize.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param size English parameter description.
/// \~russian @param size Russian parameter description.
/// \~english @param exception English parameter description.
/// \~russian @param exception Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertSize(T &&values, size_t const &size, TException const &exception) {
    static_assert(TContainerHasSizeMethod<std::decay_t<T>>::value, "Type T must have a size() method");

    auto values_size = values.size();
    if (values_size == size) {
        return std::move(values);
    } else {
        throw exception;
    }
}


template<typename T>
/// \~english @brief This function computes assertSize.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertSize.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param size English parameter description.
/// \~russian @param size Russian parameter description.
/// \~english @param error English parameter description.
/// \~russian @param error Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertSize(T &&values, size_t const &size, std::string const &error) {
    return assertSize(std::forward<T>(values), size, std::runtime_error(
        error + ", wrong items size " +
        convertion::convert<std::string>(values.size()) + ", expected " +
        convertion::convert<std::string>(size)
    ));
}


template<typename T, typename TException>
/// \~english @brief This function computes assertOne.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertOne.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param exception English parameter description.
/// \~russian @param exception Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertOne(T &&values, TException const &exception) {
    return assertSize(std::forward<T>(values), 1, exception);
}


template<typename T>
/// \~english @brief This function computes assertOne.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertOne.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param error English parameter description.
/// \~russian @param error Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertOne(T &&values, std::string const &error) {
    return assertOne(std::forward<T>(values), std::runtime_error(error));
}


template<typename T, typename TException>
/// \~english @brief This function computes assertComplete.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertComplete.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param exception English parameter description.
/// \~russian @param exception Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertComplete(T &&values, TException const &exception) {
    if (values.size() > 0)
        return std::move(values);
    else
        throw exception;
}


template<typename T>
/// \~english @brief This function computes assertComplete.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertComplete.
///     Russian detailed description (if any).
/// \~english @param values English parameter description.
/// \~russian @param values Russian parameter description.
/// \~english @param error English parameter description.
/// \~russian @param error Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::decay_t<T> assertComplete(T &&values, std::string const &error) {
    return assertComplete(std::forward<T>(values), std::runtime_error(error));
}


} // iridium


#endif // HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
