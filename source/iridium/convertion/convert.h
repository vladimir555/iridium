// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
#define HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20


#include "implementation/convert.h"


namespace iridium::convertion {

using implementation::config;

/// \~english @brief Converts a value from a source type to a target type.
/// \~russian @brief Преобразует значение из исходного типа в целевой.
/// \~english @tparam TResult The target type.
/// \~russian @tparam TResult Целевой тип.
/// \~english @tparam TValue The source type.
/// \~russian @tparam TValue Исходный тип.
/// \~english @tparam is_throwable If true, the conversion can throw exceptions.
/// \~russian @tparam is_throwable Если true, преобразование может выбрасывать исключения.
/// \~english @param value The value to convert.
/// \~russian @param value Значение для преобразования.
/// \~english @return The converted value.
/// \~russian @return Преобразованное значение.
template<typename TResult, typename TValue, bool is_throwable = false>
TResult convert(TValue const & value) {
    return implementation::TConvert<TResult, TValue, void, is_throwable>::convert(value);
}

/// \~english @brief Converts a value from a source type to a target type using a specific format.
/// \~russian @brief Преобразует значение из исходного типа в целевой с использованием определенного формата.
/// \~english @tparam TResult The target type.
/// \~russian @tparam TResult Целевой тип.
/// \~english @tparam TValue The source type.
/// \~russian @tparam TValue Исходный тип.
/// \~english @tparam TFormat The format type.
/// \~russian @tparam TFormat Тип формата.
/// \~english @tparam is_throwable If true, the conversion can throw exceptions.
/// \~russian @tparam is_throwable Если true, преобразование может выбрасывать исключения.
/// \~english @param value The value to convert.
/// \~russian @param value Значение для преобразования.
/// \~english @param format The format to use for the conversion.
/// \~russian @param format Формат, используемый для преобразования.
/// \~english @return The converted value.
/// \~russian @return Преобразованное значение.
template<typename TResult, typename TValue, typename TFormat, bool is_throwable = false>
TResult convert(TValue const & value, TFormat const & format) {
    return implementation::TConvert<TResult, TValue, void, is_throwable>::convert(value, format);
}

} // iridium::convertion


/// \~english @brief Macro to define a conversion between two types.
/// \~russian @brief Макрос для определения преобразования между двумя типами.
#define DEFINE_CONVERT(TTo, TFrom) \
namespace iridium::convertion::implementation { \
template<> \
struct TConvert<TTo, TFrom> { \
    static TTo convert(TFrom const &value); \
}; \
}


/// \~english @brief Macro to implement a conversion between two types using a function.
/// \~russian @brief Макрос для реализации преобразования между двумя типами с помощью функции.
#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
namespace iridium::convertion::implementation { \
TTo TConvert<TTo, TFrom>::convert(TFrom const &value) { \
    return TFunc(value); \
} \
}


#endif // HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
