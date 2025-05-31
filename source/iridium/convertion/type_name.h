// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TYPE_NAME_2FDF250B_2F93_405F_A121_42464C5A5B00
#define HEADER_TYPE_NAME_2FDF250B_2F93_405F_A121_42464C5A5B00


#include "implementation/type_name.h"


namespace iridium {
/// \~english @brief Namespace for type conversion utilities.
/// \~russian @brief Пространство имен для утилит преобразования типов.
namespace convertion {


/// \~english @brief Provides access to a function that retrieves a string representation of a type's name.
///     This function is brought into the `iridium::convertion` namespace from an internal implementation.
///     It is typically used as a template function, e.g., `iridium::convertion::getTypeName<MyType>()`.
///     The exact nature of the returned name (e.g., whether it is demangled, includes full namespace, etc.)
///     depends on the underlying implementation in `implementation/type_name.h`.
/// \~russian @brief Предоставляет доступ к функции, которая возвращает строковое представление имени типа.
///     Эта функция импортируется в пространство имен `iridium::convertion` из внутренней реализации.
///     Обычно используется как шаблонная функция, например, `iridium::convertion::getTypeName<MyType>()`.
///     Точный характер возвращаемого имени (например, является ли оно деманглированным, включает ли полное пространство имен и т.д.)
///     зависит от базовой реализации в `implementation/type_name.h`.
//  /// \~english @tparam T The type whose name is to be retrieved. // Cannot document template param for a using-declaration directly here.
//  /// \~russian @tparam T Тип, имя которого необходимо получить. // Невозможно документировать параметр шаблона для using-декларации напрямую здесь.
//  /// \~english @return A string representing the name of type T.
//  /// \~russian @return Строка, представляющая имя типа T.
using implementation::getTypeName;


} // convertion
} // iridium


#endif // HEADER_TYPE_NAME_2FDF250B_2F93_405F_A121_42464C5A5B00
