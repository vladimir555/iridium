// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
#define HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20


#include "implementation/convert.h"


namespace iridium {
/// \~english @brief Namespace for type conversion utilities.
///     Provides a framework for converting values between different types,
///     allowing for extensible custom conversions.
/// \~russian @brief Пространство имен для утилит преобразования типов.
///     Предоставляет фреймворк для преобразования значений между различными типами,
///     позволяя расширяемые пользовательские преобразования.
namespace convertion {


/// \~english @brief Provides access to the core conversion function template from the implementation namespace.
///     This function is the primary way to perform type conversions.
///     It typically relies on specializations of a `Convert` struct template.
///     Usage: `iridium::convertion::convert<TargetType>(sourceValue);`
/// \~russian @brief Предоставляет доступ к основной шаблонной функции преобразования из пространства имен реализации.
///     Эта функция является основным способом выполнения преобразований типов.
///     Обычно она полагается на специализации шаблонной структуры `Convert`.
///     Использование: `iridium::convertion::convert<ЦелевойТип>(исходноеЗначение);`
using implementation::convert;

/// \~english @brief Provides access to a pointer conversion function template from the implementation namespace.
///     Likely used for converting pointer types, possibly with smart pointer support or type casting.
/// \~russian @brief Предоставляет доступ к шаблонной функции преобразования указателей из пространства имен реализации.
///     Вероятно, используется для преобразования типов указателей, возможно, с поддержкой умных указателей или приведением типов.
using implementation::convertPtr;

/// \~english @brief Provides access to a configuration object or namespace for the conversion framework.
///     This might hold settings or provide utilities related to how conversions are performed.
/// \~russian @brief Предоставляет доступ к конфигурационному объекту или пространству имен для фреймворка преобразования.
///     Может содержать настройки или предоставлять утилиты, связанные с выполнением преобразований.
using implementation::config;


} // convertion
} // iridium


/// \~english @brief Defines (declares) a specialization for the `iridium::convertion::implementation::convert` function template.
///     This macro is intended to be used in header files to declare that a specific conversion
///     from `TFrom` to `TTo` will be available. The actual implementation should be provided
///     elsewhere, typically using the `IMPLEMENT_CONVERT` macro in a source file.
/// \~russian @brief Определяет (объявляет) специализацию для шаблонной функции `iridium::convertion::implementation::convert`.
///     Этот макрос предназначен для использования в заголовочных файлах для объявления доступности
///     определенного преобразования из `TFrom` в `TTo`. Фактическая реализация должна быть предоставлена
///     в другом месте, обычно с использованием макроса `IMPLEMENT_CONVERT` в файле исходного кода.
/// \~english @param TTo The target type of the conversion.
/// \~russian @param TTo Целевой тип преобразования.
/// \~english @param TFrom The source type of the conversion.
/// \~russian @param TFrom Исходный тип преобразования.
#define DEFINE_CONVERT(TTo, TFrom) \
namespace iridium { \
namespace convertion { \
namespace implementation { \
template<> \
TTo convert(TFrom const &value); \
} } }


/// \~english @brief Implements a specialization for the `iridium::convertion::implementation::convert` function template.
///     This macro provides a definition for a conversion function from `TFrom` to `TTo`,
///     using the provided function or functor `TFunc` to perform the actual conversion logic.
///     It should typically be used in a source file (.cpp) after the corresponding `DEFINE_CONVERT`
///     has been used in a header file.
/// \~russian @brief Реализует специализацию для шаблонной функции `iridium::convertion::implementation::convert`.
///     Этот макрос предоставляет определение для функции преобразования из `TFrom` в `TTo`,
///     используя предоставленную функцию или функтор `TFunc` для выполнения фактической логики преобразования.
///     Обычно его следует использовать в файле исходного кода (.cpp) после того, как соответствующий `DEFINE_CONVERT`
///     был использован в заголовочном файле.
/// \~english @param TTo The target type of the conversion.
/// \~russian @param TTo Целевой тип преобразования.
/// \~english @param TFrom The source type of the conversion.
/// \~russian @param TFrom Исходный тип преобразования.
/// \~english @param TFunc A function, functor, or lambda that takes a `TFrom const &` and returns a `TTo`.
/// \~russian @param TFunc Функция, функтор или лямбда-выражение, которое принимает `TFrom const &` и возвращает `TTo`.
#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
namespace iridium { \
namespace convertion { \
namespace implementation { \
template<> \
TTo convert(TFrom const &from) { \
    return TFunc(from); \
} \
} } }


/*
#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
template<> \
TTo iridium::convertion::implementation::convert(TFrom const &from) { \
    return TFunc(from); \
}
*/


#endif // HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
