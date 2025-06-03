// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Core implementation details for the Iridium type conversion framework.
///     Defines the primary `convert` function templates, `convertPtr` helpers,
///     and declarations for numerous explicit conversion specializations.
///     This header is typically not included directly by end-users.
/// \~russian @file
/// @brief Основные детали реализации для фреймворка преобразования типов Iridium.
///     Определяет основные шаблонные функции `convert`, вспомогательные функции `convertPtr`
///     и объявления для многочисленных явных специализаций преобразования.
///     Этот заголовочный файл обычно не включается напрямую конечными пользователями.

#ifndef HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
#define HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709


#include <list>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <stdexcept>


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Namespace for type conversion utilities. (Implementation details reside in `implementation` namespace)
/// \~russian @brief Пространство имен для утилит преобразования типов. (Детали реализации находятся в пространстве имен `implementation`)
namespace convertion {
/// \~english @brief Contains the internal implementation details for the type conversion framework.
///     Users should typically not use elements from this namespace directly, but rather access them
///     via `iridium::convertion`.
/// \~russian @brief Содержит внутренние детали реализации для фреймворка преобразования типов.
///     Пользователи обычно не должны использовать элементы из этого пространства имен напрямую,
///     а получать к ним доступ через `iridium::convertion`.
namespace implementation {


/// \~english @brief Configuration settings for the conversion framework.
///     Note: A TODO comment in the source ("not good for multithreading") suggests potential issues
///     with thread safety regarding how these settings are accessed or initialized.
/// \~russian @brief Настройки конфигурации для фреймворка преобразования.
///     Примечание: Комментарий TODO в исходном коде ("not good for multithreading") предполагает
///     потенциальные проблемы с потокобезопасностью в отношении доступа или инициализации этих настроек.
struct config {
    /// \~english @brief Controls the precision used when converting double-precision floating-point numbers to strings.
    ///     This is an atomic integer, but its usage context might have thread-safety issues as per source code TODO.
    /// \~russian @brief Управляет точностью, используемой при преобразовании чисел с плавающей запятой двойной точности в строки.
    ///     Это атомарное целое число, но контекст его использования может иметь проблемы с потокобезопасностью согласно TODO в исходном коде.
    static std::atomic<int> double_precission;
};


// ----- interface

/// \~english @brief Base template for type conversion. This unspecialized version is called when no specific conversion specialization is found.
///     It throws a `std::runtime_error` to indicate that the requested conversion is not implemented.
///     To enable a conversion, a template specialization must be provided for the specific `TResult` and `TValue` types.
/// \~russian @brief Базовый шаблон для преобразования типов. Эта неспециализированная версия вызывается, когда не найдена конкретная специализация преобразования.
///     Она выбрасывает `std::runtime_error`, указывая, что запрошенное преобразование не реализовано.
///     Для включения преобразования необходимо предоставить специализацию шаблона для конкретных типов `TResult` и `TValue`.
/// \~english @tparam TResult The target type to convert to.
/// \~russian @tparam TResult Целевой тип, в который выполняется преобразование.
/// \~english @tparam TValue The source type to convert from.
/// \~russian @tparam TValue Исходный тип, из которого выполняется преобразование.
/// \~english @param value The source value to convert (unused in this base template).
/// \~russian @param value Исходное значение для преобразования (не используется в этом базовом шаблоне).
/// \~english @return This function always throws and does not return a value.
/// \~russian @return Эта функция всегда выбрасывает исключение и не возвращает значение.
/// \~english @throws std::runtime_error Indicates that no specific conversion specialization was found.
/// \~russian @throws std::runtime_error Указывает, что не найдена конкретная специализация преобразования.
template<typename TResult, typename TValue>
TResult convert(TValue const &) {
    throw std::runtime_error("convertion error: no specialization exists for this type");
}

/// \~english @brief Overloaded base template for type conversion with an additional formatting parameter.
///     This declaration suggests that conversions can be influenced by a format specifier.
///     Like the primary `convert` template, it relies on explicit specializations for actual functionality.
///     If no specialization matches, linking will likely fail, or it might fall back to a less specific `convert` if TFormat is convertible.
/// \~russian @brief Перегруженный базовый шаблон для преобразования типов с дополнительным параметром форматирования.
///     Это объявление предполагает, что на преобразования может влиять спецификатор формата.
///     Как и основной шаблон `convert`, он полагается на явные специализации для фактической функциональности.
///     Если соответствующая специализация не найдена, скорее всего, произойдет ошибка компоновки, или может произойти откат к менее специфичной версии `convert`, если TFormat является преобразуемым.
/// \~english @tparam TResult The target type to convert to.
/// \~russian @tparam TResult Целевой тип, в который выполняется преобразование.
/// \~english @tparam TValue The source type to convert from.
/// \~russian @tparam TValue Исходный тип, из которого выполняется преобразование.
/// \~english @tparam TFormat The type of the formatting parameter.
/// \~russian @tparam TFormat Тип параметра форматирования.
/// \~english @param value The source value to convert.
/// \~russian @param value Исходное значение для преобразования.
/// \~english @param format The formatting specifier to influence the conversion.
/// \~russian @param format Спецификатор форматирования для влияния на преобразование.
/// \~english @return The converted value of type `TResult`.
/// \~russian @return Преобразованное значение типа `TResult`.
template<typename TResult, typename TValue, typename TFormat>
TResult convert(TValue const &value, TFormat const &format); // No base definition, relies on specializations


/// \~english @brief Converts a value to a `std::shared_ptr<TResult>`.
///     This template likely first converts the input `value` to `TResult` using the primary `convert` mechanism,
///     and then wraps the result in a `std::shared_ptr`.
/// \~russian @brief Преобразует значение в `std::shared_ptr<TResult>`.
///     Этот шаблон, вероятно, сначала преобразует входное `value` в `TResult` с использованием основного механизма `convert`,
///     а затем оборачивает результат в `std::shared_ptr`.
/// \~english @tparam TResult The target type for the value pointed to by the shared_ptr.
/// \~russian @tparam TResult Целевой тип для значения, на которое указывает shared_ptr.
/// \~english @tparam TValue The source type of the value to convert.
/// \~russian @tparam TValue Исходный тип значения для преобразования.
/// \~english @param value The source value to convert and wrap in a shared_ptr.
/// \~russian @param value Исходное значение для преобразования и упаковки в shared_ptr.
/// \~english @return A `std::shared_ptr<TResult>` holding the converted value.
/// \~russian @return `std::shared_ptr<TResult>`, содержащий преобразованное значение.
template<typename TResult, typename TValue>
std::shared_ptr<TResult> convertPtr(TValue const &value) {
    return std::make_shared<TResult>(convert<TResult>(value));
}

/// \~english @brief Converts a `std::string` to a `std::shared_ptr<TResult>`.
///     If the input string is "NULL", it returns a null `shared_ptr`. Otherwise, it converts
///     the string to `TResult` and wraps it in a `shared_ptr`.
/// \~russian @brief Преобразует `std::string` в `std::shared_ptr<TResult>`.
///     Если входная строка равна "NULL", возвращает нулевой `shared_ptr`. В противном случае преобразует
///     строку в `TResult` и оборачивает ее в `shared_ptr`.
/// \~english @tparam TResult The target type for the value pointed to by the shared_ptr.
/// \~russian @tparam TResult Целевой тип для значения, на которое указывает shared_ptr.
/// \~english @param value The source string to convert. Special value "NULL" results in a nullptr.
/// \~russian @param value Исходная строка для преобразования. Специальное значение "NULL" приводит к nullptr.
/// \~english @return A `std::shared_ptr<TResult>` holding the converted value, or nullptr if input was "NULL".
/// \~russian @return `std::shared_ptr<TResult>`, содержащий преобразованное значение, или nullptr, если входная строка была "NULL".
template<typename TResult>
std::shared_ptr<TResult> convertPtr(std::string const &value) {
    if (value == "NULL")
        return std::shared_ptr<TResult>(nullptr);

    return std::make_shared<TResult>(convert<TResult>(value));
}

/// \~english @brief Converts a `std::shared_ptr<TValue>` to its string representation.
///     If the `shared_ptr` is null, returns the string "NULL". Otherwise, dereferences the pointer
///     and converts the pointed-to `TValue` to `std::string`.
/// \~russian @brief Преобразует `std::shared_ptr<TValue>` в его строковое представление.
///     Если `shared_ptr` равен null, возвращает строку "NULL". В противном случае разыменовывает указатель
///     и преобразует значение `TValue`, на которое он указывает, в `std::string`.
/// \~english @tparam TValue The type of the value held by the shared_ptr.
/// \~russian @tparam TValue Тип значения, содержащегося в shared_ptr.
/// \~english @param value The `shared_ptr` whose pointed-to value is to be converted to a string.
/// \~russian @param value `shared_ptr`, значение которого (на которое он указывает) должно быть преобразовано в строку.
/// \~english @return A string representation of the value, or "NULL" if the pointer is null.
/// \~russian @return Строковое представление значения или "NULL", если указатель равен null.
template<typename TValue>
std::string convertPtr(std::shared_ptr<TValue> const &value) {
    if (value)
        return convert<std::string>(*value);
    else
        return "NULL";
}

/// \~english @brief Converts a `std::shared_ptr<TValue>` to its string representation using a format specifier.
///     If the `shared_ptr` is null, returns "NULL". Otherwise, dereferences the pointer
///     and converts the pointed-to `TValue` to `std::string` using the provided format.
/// \~russian @brief Преобразует `std::shared_ptr<TValue>` в его строковое представление с использованием спецификатора формата.
///     Если `shared_ptr` равен null, возвращает "NULL". В противном случае разыменовывает указатель
///     и преобразует значение `TValue`, на которое он указывает, в `std::string`, используя предоставленный формат.
/// \~english @tparam TValue The type of the value held by the shared_ptr.
/// \~russian @tparam TValue Тип значения, содержащегося в shared_ptr.
/// \~english @tparam TFormat The type of the formatting parameter.
/// \~russian @tparam TFormat Тип параметра форматирования.
/// \~english @param value The `shared_ptr` whose pointed-to value is to be converted.
/// \~russian @param value `shared_ptr`, значение которого (на которое он указывает) должно быть преобразовано.
/// \~english @param format The formatting specifier.
/// \~russian @param format Спецификатор форматирования.
/// \~english @return A formatted string representation of the value, or "NULL" if the pointer is null.
/// \~russian @return Отформатированное строковое представление значения или "NULL", если указатель равен null.
template<typename TValue, typename TFormat>
std::string convertPtr(std::shared_ptr<TValue> const &value, TFormat const &format) {
    if (value)
        return convert<std::string>(*value, format);
    else
        return "NULL";
}


// ----- implementation (Declarations of specializations)

// Note: The following are declarations of explicit specializations of the `convert` function template.
// The actual definitions are expected to be in corresponding .cpp files or defined via macros like IMPLEMENT_CONVERT.

/// \~english @brief Converts a `std::chrono::system_clock::time_point` to `std::string` with GMT option.
/// \~russian @brief Преобразует `std::chrono::system_clock::time_point` в `std::string` с опцией GMT.
template<> std::string convert(std::chrono::system_clock::time_point const &value, bool const &is_gmt_time);
/// \~english @brief Converts a `std::chrono::system_clock::time_point` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::system_clock::time_point` в `std::string`.
template<> std::string convert(std::chrono::system_clock::time_point const &value);
/// \~english @brief Converts `std::chrono::hours` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::hours` в `std::string`.
template<> std::string convert(std::chrono::hours const &value);
/// \~english @brief Converts `std::chrono::minutes` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::minutes` в `std::string`.
template<> std::string convert(std::chrono::minutes const &value);
/// \~english @brief Converts `std::chrono::seconds` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::seconds` в `std::string`.
template<> std::string convert(std::chrono::seconds const &value);
/// \~english @brief Converts `std::chrono::milliseconds` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::milliseconds` в `std::string`.
template<> std::string convert(std::chrono::milliseconds const &value);
/// \~english @brief Converts `std::chrono::microseconds` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::microseconds` в `std::string`.
template<> std::string convert(std::chrono::microseconds const &value);
/// \~english @brief Converts `std::chrono::nanoseconds` to `std::string`.
/// \~russian @brief Преобразует `std::chrono::nanoseconds` в `std::string`.
template<> std::string convert(std::chrono::nanoseconds const &value);
/// \~english @brief Converts `bool` to `std::string` (e.g., "true" or "false").
/// \~russian @brief Преобразует `bool` в `std::string` (например, "true" или "false").
template<> std::string convert(bool const &value);
/// \~english @brief Converts `int64_t` to `std::string` using a specific base.
/// \~russian @brief Преобразует `int64_t` в `std::string` с использованием указанного основания.
template<> std::string convert(int64_t const &value, int const &base);
/// \~english @brief Converts `uint64_t` to `std::string` using a specific base.
/// \~russian @brief Преобразует `uint64_t` в `std::string` с использованием указанного основания.
template<> std::string convert(uint64_t const &value, int const &base);
/// \~english @brief Converts `int32_t` to `std::string` using a specific base.
/// \~russian @brief Преобразует `int32_t` в `std::string` с использованием указанного основания.
template<> std::string convert(int32_t const &value, int const &base);
/// \~english @brief Converts `uint32_t` to `std::string` using a specific base.
/// \~russian @brief Преобразует `uint32_t` в `std::string` с использованием указанного основания.
template<> std::string convert(uint32_t const &value, int const &base);
/// \~english @brief Converts `int64_t` to `std::string` (typically base 10).
/// \~russian @brief Преобразует `int64_t` в `std::string` (обычно по основанию 10).
template<> std::string convert(int64_t const &value);
/// \~english @brief Converts `uint64_t` to `std::string` (typically base 10).
/// \~russian @brief Преобразует `uint64_t` в `std::string` (обычно по основанию 10).
template<> std::string convert(uint64_t const &value);
/// \~english @brief Converts `int32_t` to `std::string` (typically base 10).
/// \~russian @brief Преобразует `int32_t` в `std::string` (обычно по основанию 10).
template<> std::string convert(int32_t const &value);
/// \~english @brief Converts `uint32_t` to `std::string` (typically base 10).
/// \~russian @brief Преобразует `uint32_t` в `std::string` (обычно по основанию 10).
template<> std::string convert(uint32_t const &value);
/// \~english @brief Converts `int16_t` to `std::string`.
/// \~russian @brief Преобразует `int16_t` в `std::string`.
template<> std::string convert(int16_t const &value);
/// \~english @brief Converts `uint16_t` to `std::string`.
/// \~russian @brief Преобразует `uint16_t` в `std::string`.
template<> std::string convert(uint16_t const &value);
/// \~english @brief Converts `uint8_t` (often a character type) to `std::string`.
/// \~russian @brief Преобразует `uint8_t` (часто символьный тип) в `std::string`.
template<> std::string convert(uint8_t const &value);
/// \~english @brief Converts `double` to `std::string` with specified precision.
/// \~russian @brief Преобразует `double` в `std::string` с указанной точностью.
template<> std::string convert(double const &value, int const &precision);
/// \~english @brief Converts `double` to `std::string` (using default or configured precision).
/// \~russian @brief Преобразует `double` в `std::string` (используя точность по умолчанию или настроенную).
template<> std::string convert(double const &value);
/// \~english @brief Converts `float` to `std::string` with specified precision.
/// \~russian @brief Преобразует `float` в `std::string` с указанной точностью.
template<> std::string convert(float const &value, int const &precision);
/// \~english @brief Converts `float` to `std::string` (using default or configured precision).
/// \~russian @brief Преобразует `float` в `std::string` (используя точность по умолчанию или настроенную).
template<> std::string convert(float const &value);
/// \~english @brief Converts `std::thread::id` to `std::string`.
/// \~russian @brief Преобразует `std::thread::id` в `std::string`.
template<> std::string convert(std::thread::id const &value);
/// \~english @brief Converts `std::exception` to `std::string` (likely by calling `what()`).
/// \~russian @brief Преобразует `std::exception` в `std::string` (вероятно, путем вызова `what()`).
template<> std::string convert(std::exception const &value);
// This seems like a duplicate declaration from above. Assuming it's an oversight or for a different context not apparent here.
// template<> std::string convert(std::exception const &value);
/// \~english @brief Converts `std::nested_exception` to `std::string`.
/// \~russian @brief Преобразует `std::nested_exception` в `std::string`.
template<> std::string convert(std::nested_exception const &value);
/// \~english @brief Converts `std::string` to `std::chrono::system_clock::time_point`.
/// \~russian @brief Преобразует `std::string` в `std::chrono::system_clock::time_point`.
template<> std::chrono::system_clock::time_point convert(std::string const &value);
/// \~english @brief Converts `std::string` to `bool` (e.g., "true", "1" to true; "false", "0" to false).
/// \~russian @brief Преобразует `std::string` в `bool` (например, "true", "1" в true; "false", "0" в false).
template<> bool convert(std::string const &value_);
/// \~english @brief Converts `std::string` to `int32_t`.
/// \~russian @brief Преобразует `std::string` в `int32_t`.
template<> int32_t convert(std::string const &value);
/// \~english @brief Converts `std::string` to `int64_t`.
/// \~russian @brief Преобразует `std::string` в `int64_t`.
template<> int64_t convert(std::string const &value);
/// \~english @brief Converts `std::string` to `uint32_t`.
/// \~russian @brief Преобразует `std::string` в `uint32_t`.
template<> uint32_t convert(std::string const &value);
/// \~english @brief Converts `std::string` to `uint16_t`.
/// \~russian @brief Преобразует `std::string` в `uint16_t`.
template<> uint16_t convert(std::string const &value);
/// \~english @brief Converts `std::string` to `uint8_t`.
/// \~russian @brief Преобразует `std::string` в `uint8_t`.
template<> uint8_t convert(std::string const &value);
/// \~english @brief Converts `std::string` to `uint64_t`.
/// \~russian @brief Преобразует `std::string` в `uint64_t`.
template<> uint64_t convert(std::string const &value); // todo: string -> uint with base
/// \~english @brief Converts `std::string` to `double`.
/// \~russian @brief Преобразует `std::string` в `double`.
template<> double convert(std::string const &value);
/// \~english @brief Converts `std::string` to `float`.
/// \~russian @brief Преобразует `std::string` в `float`.
template<> float convert(std::string const &value);
/// \~english @brief Converts `std::wstring` to `std::string`.
/// \~russian @brief Преобразует `std::wstring` в `std::string`.
template<> std::string convert(std::wstring const &value);
/// \~english @brief Converts `std::string` to `std::wstring`.
/// \~russian @brief Преобразует `std::string` в `std::wstring`.
template<> std::wstring convert(std::string const &value);
/// \~english @brief Converts `std::u16string` to `std::string`.
/// \~russian @brief Преобразует `std::u16string` в `std::string`.
template<> std::string convert(std::u16string const &value);
/// \~english @brief Converts `std::u32string` to `std::string`.
/// \~russian @brief Преобразует `std::u32string` в `std::string`.
template<> std::string convert(std::u32string const &value);
/// \~english @brief Converts `std::string` to `std::u16string`.
/// \~russian @brief Преобразует `std::string` в `std::u16string`.
template<> std::u16string convert(std::string const &value);
/// \~english @brief Converts `std::string` to `std::u32string`.
/// \~russian @brief Преобразует `std::string` в `std::u32string`.
template<> std::u32string convert(std::string const &value);
/// \~english @brief Converts `std::string` to `std::string` (identity conversion).
/// \~russian @brief Преобразует `std::string` в `std::string` (тождественное преобразование).
template<> std::string convert(std::string const &value);


template<>
std::string convert(std::list<std::string> const &values);


} // implementation
} // convertion
} // iridium


#endif // HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
