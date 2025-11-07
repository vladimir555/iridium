// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
#define HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED


#include <stdexcept>
#include <string>

#include "iridium/convertion/convert.h"


namespace iridium {


/// \~english @brief Asserts that a value (e.g., a boolean, integer, or other type convertible to bool) is "truthy", throwing a custom exception if it is not.
/// \~russian @brief Утверждает, что значение (например, булево, целочисленное или другого типа, приводимого к bool) является "истинным", и выбрасывает пользовательское исключение, если это не так.
/// \~english @tparam T The type of the value to check. It must be convertible to `bool`.
/// \~russian @tparam T Тип проверяемого значения. Он должен быть приводим к `bool`.
/// \~english @tparam TException The type of the exception to throw if the assertion fails.
/// \~russian @tparam TException Тип исключения, которое будет выброшено в случае сбоя утверждения.
/// \~english @param value The value to check for existence or truthiness.
/// \~russian @param value Значение для проверки на существование или истинность.
/// \~english @param exception The exception object to throw if `value` evaluates to `false`.
/// \~russian @param exception Объект исключения, который будет выброшен, если `value` оценивается как `false`.
/// \~english @return The original `value` if the assertion passes.
/// \~russian @return Исходное `value`, если утверждение проходит.
/// \~english @throw TException if `static_cast<bool>(value)` is `false`.
/// \~russian @throw TException если `static_cast<bool>(value)` равно `false`.
template<typename T, typename TException>
T assertExists(T value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


/// \~english @brief Asserts that a value is "truthy", throwing a `std::runtime_error` with a specified message if it is not.
/// \~russian @brief Утверждает, что значение является "истинным", и выбрасывает `std::runtime_error` с указанным сообщением, если это не так.
/// \~english @tparam T The type of the value to check.
/// \~russian @tparam T Тип проверяемого значения.
/// \~english @param value The value to check.
/// \~russian @param value Значение для проверки.
/// \~english @param error The error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке для `std::runtime_error`.
/// \~english @return The original `value` if the assertion passes.
/// \~russian @return Исходное `value`, если утверждение проходит.
template<typename T>
T assertExists(T value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


/// \~english @brief Asserts that a value is "truthy", throwing a `std::runtime_error` with a specified message if it is not.
/// \~russian @brief Утверждает, что значение является "истинным", и выбрасывает `std::runtime_error` с указанным сообщением, если это не так.
/// \~english @tparam T The type of the value to check.
/// \~russian @tparam T Тип проверяемого значения.
/// \~english @param value The value to check.
/// \~russian @param value Значение для проверки.
/// \~english @param error The C-style string error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке в виде C-строки для `std::runtime_error`.
/// \~english @return The original `value` if the assertion passes.
/// \~russian @return Исходное `value`, если утверждение проходит.
template<typename T>
T assertExists(T value, char const *error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


/// \~english @brief Asserts that a raw pointer is not null, throwing a custom exception if it is.
/// \~russian @brief Утверждает, что "сырой" указатель не является нулевым, и выбрасывает пользовательское исключение, если это так.
/// \~english @tparam T The type pointed to by the pointer.
/// \~russian @tparam T Тип, на который указывает указатель.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param value The pointer to check.
/// \~russian @param value Указатель для проверки.
/// \~english @param exception The exception to throw if `value` is null.
/// \~russian @param exception Исключение, которое будет выброшено, если `value` равно null.
/// \~english @return The original `value` if it is not null.
/// \~russian @return Исходное `value`, если оно не равно null.
template<typename T, typename TException>
T *assertExists(T *value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


/// \~english @brief Asserts that a raw pointer is not null, throwing a `std::runtime_error` if it is.
/// \~russian @brief Утверждает, что "сырой" указатель не является нулевым, и выбрасывает `std::runtime_error`, если это так.
/// \~english @tparam T The type pointed to by the pointer.
/// \~russian @tparam T Тип, на который указывает указатель.
/// \~english @param value The pointer to check.
/// \~russian @param value Указатель для проверки.
/// \~english @param error The error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке для `std::runtime_error`.
/// \~english @return The original `value` if it is not null.
/// \~russian @return Исходное `value`, если оно не равно null.
template<typename T>
T *assertExists(T *value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


/// \~english @brief Asserts that a raw pointer is not null, throwing a `std::runtime_error` if it is.
/// \~russian @brief Утверждает, что "сырой" указатель не является нулевым, и выбрасывает `std::runtime_error`, если это так.
/// \~english @tparam T The type pointed to by the pointer.
/// \~russian @tparam T Тип, на который указывает указатель.
/// \~english @param value The pointer to check.
/// \~russian @param value Указатель для проверки.
/// \~english @param error The C-style string error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке в виде C-строки для `std::runtime_error`.
/// \~english @return The original `value` if it is not null.
/// \~russian @return Исходное `value`, если оно не равно null.
template<typename T>
T *assertExists(T *value, char const *error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


/// \~english @brief Asserts that a `std::shared_ptr` is not empty, throwing a custom exception if it is.
/// \~russian @brief Утверждает, что `std::shared_ptr` не пуст, и выбрасывает пользовательское исключение, если это так.
/// \~english @tparam T The type managed by the shared pointer.
/// \~russian @tparam T Тип, управляемый разделяемым указателем.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param value The `std::shared_ptr` to check.
/// \~russian @param value `std::shared_ptr` для проверки.
/// \~english @param exception The exception to throw if `value` is empty.
/// \~russian @param exception Исключение, которое будет выброшено, если `value` пуст.
/// \~english @return The original `value` if it is not empty.
/// \~russian @return Исходное `value`, если оно не пустое.
template<typename T, typename TException>
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, TException const &exception) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw exception; // ----->
}


/// \~english @brief Asserts that a `std::shared_ptr` is not empty, throwing a `std::runtime_error` if it is.
/// \~russian @brief Утверждает, что `std::shared_ptr` не пуст, и выбрасывает `std::runtime_error`, если это так.
/// \~english @tparam T The type managed by the shared pointer.
/// \~russian @tparam T Тип, управляемый разделяемым указателем.
/// \~english @param value The `std::shared_ptr` to check.
/// \~russian @param value `std::shared_ptr` для проверки.
/// \~english @param error The error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке для `std::runtime_error`.
/// \~english @return The original `value` if it is not empty.
/// \~russian @return Исходное `value`, если оно не пустое.
template<typename T>
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, std::string const &error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


/// \~english @brief Asserts that a `std::shared_ptr` is not empty, throwing a `std::runtime_error` if it is.
/// \~russian @brief Утверждает, что `std::shared_ptr` не пуст, и выбрасывает `std::runtime_error`, если это так.
/// \~english @tparam T The type managed by the shared pointer.
/// \~russian @tparam T Тип, управляемый разделяемым указателем.
/// \~english @param value The `std::shared_ptr` to check.
/// \~russian @param value `std::shared_ptr` для проверки.
/// \~english @param error The C-style string error message for the `std::runtime_error`.
/// \~russian @param error Сообщение об ошибке в виде C-строки для `std::runtime_error`.
/// \~english @return The original `value` if it is not empty.
/// \~russian @return Исходное `value`, если оно не пустое.
template<typename T>
std::shared_ptr<T> assertExists(std::shared_ptr<T> const &value, char const *error) {
    return assertExists(value, std::runtime_error(error)); // ----->
}


template<typename T, typename = void>
struct TContainerHasSizeMethod: std::false_type {};


template<typename T>
struct TContainerHasSizeMethod<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};


/// \~english @brief Asserts that a container has a specific size, throwing a custom exception if it does not.
/// \~russian @brief Утверждает, что контейнер имеет определенный размер, и выбрасывает пользовательское исключение, если это не так.
/// \~english @tparam T The type of the container, which must have a `size()` method.
/// \~russian @tparam T Тип контейнера, который должен иметь метод `size()`.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param values The container to check. Passed as a forwarding reference.
/// \~russian @param values Контейнер для проверки. Передается как универсальная ссылка.
/// \~english @param size The expected size of the container.
/// \~russian @param size Ожидаемый размер контейнера.
/// \~english @param exception The exception to throw if the size does not match.
/// \~russian @param exception Исключение, которое будет выброшено, если размер не совпадает.
/// \~english @return The original container, moved if it was an rvalue.
/// \~russian @return Исходный контейнер, перемещенный, если он был rvalue.
template<typename T, typename TException>
[[nodiscard]] std::decay_t<T> assertSize(T &&values, size_t const &size, TException const &exception) {
    static_assert(TContainerHasSizeMethod<std::decay_t<T>>::value, "Type T must have a size() method");

    auto values_size = values.size();
    if (values_size == size) {
        return std::move(values);
    } else {
        throw exception;
    }
}


/// \~english @brief Asserts that a container has a specific size, throwing a `std::runtime_error` with a detailed message if it does not.
/// \~russian @brief Утверждает, что контейнер имеет определенный размер, и выбрасывает `std::runtime_error` с подробным сообщением, если это не так.
/// \~english @tparam T The type of the container.
/// \~russian @tparam T Тип контейнера.
/// \~english @param values The container to check.
/// \~russian @param values Контейнер для проверки.
/// \~english @param size The expected size.
/// \~russian @param size Ожидаемый размер.
/// \~english @param error The base error message.
/// \~russian @param error Основное сообщение об ошибке.
/// \~english @return The original container.
/// \~russian @return Исходный контейнер.
template<typename T>
[[nodiscard]] std::decay_t<T> assertSize(T &&values, size_t const &size, std::string const &error) {
    return assertSize(std::forward<T>(values), size, std::runtime_error(
        error + ", wrong items size " +
        convertion::convert<std::string>(values.size()) + ", expected " +
        convertion::convert<std::string>(size)
    ));
}


/// \~english @brief Asserts that a container has exactly one element, throwing a custom exception if it does not.
/// \~russian @brief Утверждает, что контейнер содержит ровно один элемент, и выбрасывает пользовательское исключение, если это не так.
/// \~english @tparam T The type of the container.
/// \~russian @tparam T Тип контейнера.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param values The container to check.
/// \~russian @param values Контейнер для проверки.
/// \~english @param exception The exception to throw if the size is not 1.
/// \~russian @param exception Исключение, которое будет выброшено, если размер не равен 1.
/// \~english @return The original container.
/// \~russian @return Исходный контейнер.
template<typename T, typename TException>
[[nodiscard]] std::decay_t<T> assertOne(T &&values, TException const &exception) {
    return assertSize(std::forward<T>(values), 1, exception);
}


/// \~english @brief Asserts that a container has exactly one element, throwing a `std::runtime_error` if it does not.
/// \~russian @brief Утверждает, что контейнер содержит ровно один элемент, и выбрасывает `std::runtime_error`, если это не так.
/// \~english @tparam T The type of the container.
/// \~russian @tparam T Тип контейнера.
/// \~english @param values The container to check.
/// \~russian @param values Контейнер для проверки.
/// \~english @param error The error message.
/// \~russian @param error Сообщение об ошибке.
/// \~english @return The original container.
/// \~russian @return Исходный контейнер.
template<typename T>
[[nodiscard]] std::decay_t<T> assertOne(T &&values, std::string const &error) {
    return assertOne(std::forward<T>(values), std::runtime_error(error));
}


/// \~english @brief Asserts that a container is not empty, throwing a custom exception if it is.
/// \~russian @brief Утверждает, что контейнер не пуст, и выбрасывает пользовательское исключение, если это так.
/// \~english @tparam T The type of the container.
/// \~russian @tparam T Тип контейнера.
/// \~english @tparam TException The type of the exception to throw.
/// \~russian @tparam TException Тип выбрасываемого исключения.
/// \~english @param values The container to check.
/// \~russian @param values Контейнер для проверки.
/// \~english @param exception The exception to throw if the container is empty.
/// \~russian @param exception Исключение, которое будет выброшено, если контейнер пуст.
/// \~english @return The original container.
/// \~russian @return Исходный контейнер.
template<typename T, typename TException>
[[nodiscard]] std::decay_t<T> assertComplete(T &&values, TException const &exception) {
    if (values.size() > 0)
        return std::move(values);
    else
        throw exception;
}


/// \~english @brief Asserts that a container is not empty, throwing a `std::runtime_error` if it is.
/// \~russian @brief Утверждает, что контейнер не пуст, и выбрасывает `std::runtime_error`, если это так.
/// \~english @tparam T The type of the container.
/// \~russian @tparam T Тип контейнера.
/// \~english @param values The container to check.
/// \~russian @param values Контейнер для проверки.
/// \~english @param error The error message.
/// \~russian @param error Сообщение об ошибке.
/// \~english @return The original container.
/// \~russian @return Исходный контейнер.
template<typename T>
[[nodiscard]] std::decay_t<T> assertComplete(T &&values, std::string const &error) {
    return assertComplete(std::forward<T>(values), std::runtime_error(error));
}


} // iridium


#endif // HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
