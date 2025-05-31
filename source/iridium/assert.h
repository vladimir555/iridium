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
T &&assertExists(T &&value, TException const &exception) {
    if (static_cast<bool>(value))
        return std::forward<T>(value); // ----->
    else
        throw exception; // ----->
}


template<typename T>
T &&assertExists(T &&value, std::string const &error) {
    return assertExists(std::forward<T>(value), std::runtime_error(error)); // ----->
}


template<typename T, typename TException>
T *assertExists(T *value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


template<typename T>
T *assertExists(T *value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


template<typename T, typename TException>
/// \~english @brief This function computes assertExists.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertExists.
///     Russian detailed description (if any).
/// \~english @param value English parameter description.
/// \~russian @param value Russian parameter description.
/// \~english @param exception English parameter description.
/// \~russian @param exception Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


template<typename T>
/// \~english @brief This function computes assertExists.
///     English detailed description (if any).
/// \~russian @brief Эта функция computes assertExists.
///     Russian detailed description (if any).
/// \~english @param value English parameter description.
/// \~russian @param value Russian parameter description.
/// \~english @param error English parameter description.
/// \~russian @param error Russian parameter description.
/// \~english @return English return value description.
/// \~russian @return Russian return value description.
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


template<typename T, typename = void>
struct TContainerHasSizeMethod: std::false_type {};


template<typename T>
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
