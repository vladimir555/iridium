// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Core implementation details for retrieving type names as strings.
///     Defines the `getTypeName` function template declaration and the `IMPLEMENT_TYPE_NAME` macro
///     for generating specializations. This header is typically not included directly by end-users.
/// \~russian @file
/// @brief Основные детали реализации для получения имен типов в виде строк.
///     Определяет объявление шаблонной функции `getTypeName` и макрос `IMPLEMENT_TYPE_NAME`
///     для генерации специализаций. Этот заголовочный файл обычно не включается напрямую конечными пользователями.

#ifndef HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E
#define HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E


#include <string>


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Namespace for type conversion utilities. (Implementation details reside in `implementation` namespace)
/// \~russian @brief Пространство имен для утилит преобразования типов. (Детали реализации находятся в пространстве имен `implementation`)
namespace convertion {
/// \~english @brief Contains the internal implementation details for type name retrieval.
///     Users should typically not use elements from this namespace directly, but rather access them
///     via `iridium::convertion::getTypeName`.
/// \~russian @brief Содержит внутренние детали реализации для получения имени типа.
///     Пользователи обычно не должны использовать элементы из этого пространства имен напрямую,
///     а получать к ним доступ через `iridium::convertion::getTypeName`.
namespace implementation {


/// \~english @brief Retrieves a string representation of the name of type T.
///     This is a template function declaration. Actual type names are provided by
///     explicit specializations, typically generated using the `IMPLEMENT_TYPE_NAME` macro.
///     If no specialization is found for a given type `T`, a link-time error will occur.
/// \~russian @brief Получает строковое представление имени типа T.
///     Это объявление шаблонной функции. Фактические имена типов предоставляются
///     явными специализациями, обычно генерируемыми с помощью макроса `IMPLEMENT_TYPE_NAME`.
///     Если для данного типа `T` не найдена специализация, произойдет ошибка времени компоновки.
/// \~english @tparam T The type whose name is to be retrieved.
/// \~russian @tparam T Тип, имя которого необходимо получить.
/// \~english @return A `std::string` containing the name of type `T` as defined by a specialization.
/// \~russian @return `std::string`, содержащая имя типа `T`, как определено специализацией.
template<typename T>
std::string getTypeName();


} // implementation
} // convertion
} // iridium


/// \~english @brief Implements a specialization of `iridium::convertion::implementation::getTypeName<type>()`.
///     This macro generates a function specialization that returns the stringified version of the `type` argument.
///     For example, `IMPLEMENT_TYPE_NAME(int)` would generate a function that returns "int".
///     `IMPLEMENT_TYPE_NAME(MyNamespace::MyClass)` would return "MyNamespace::MyClass".
///     This means the "name" is exactly as written in the macro argument, not a compiler-mangled or demangled name.
/// \~russian @brief Реализует специализацию `iridium::convertion::implementation::getTypeName<type>()`.
///     Этот макрос генерирует специализацию функции, которая возвращает строковое представление аргумента `type`.
///     Например, `IMPLEMENT_TYPE_NAME(int)` сгенерирует функцию, возвращающую "int".
///     `IMPLEMENT_TYPE_NAME(MyNamespace::MyClass)` вернет "MyNamespace::MyClass".
///     Это означает, что "имя" будет точно таким, как оно написано в аргументе макроса, а не искаженным компилятором или деманглированным именем.
/// \~english @param type The type for which to generate the `getTypeName` specialization. This should be a valid C++ type name.
/// \~russian @param type Тип, для которого генерируется специализация `getTypeName`. Это должно быть допустимое имя типа C++.
#define IMPLEMENT_TYPE_NAME(type) \
namespace iridium { \
namespace convertion { \
namespace implementation { \
template<> \
std::string getTypeName<type>() { \
    return #type; \
}}}}


#endif // HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E
