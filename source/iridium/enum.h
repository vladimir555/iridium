// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Provides macros for creating enhanced C++ enumerations with features like string conversion and iteration.
/// \~russian @file
/// @brief Предоставляет макросы для создания расширенных перечислений C++ с такими функциями, как преобразование в строку и итерация.

#ifndef HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
#define HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C


#include "iridium/strings.h"
#include "iridium/convertion/convert.h"

#include <list>
#include <map>
#include <iostream>
#include <limits.h>


#ifdef _MSC_VER
#define UNUSED
#define DISABLE_C26827 __pragma(warning(push)) __pragma(warning(suppress: 26827))
#define ENABLE_C26827  __pragma(warning(pop))
#else
#define UNUSED __attribute__((unused))
#define DISABLE_C26827
#define ENABLE_C26827
#endif // _MSC_VER


/// \~english @brief Defines an enhanced enumeration class.
///     This macro generates a class that wraps a standard C++ enum, providing
///     additional functionalities such as conversion to/from std::string, iteration
///     over enum values, and construction from integer or string representations.
///     It automatically creates an internal enum `TEnumInternal`, constructors,
///     conversion operators, `getEnums()` static method to retrieve all enum values,
///     `convert()` static methods for string/enum conversion, and
///     `convertToFlagsString()` for bitmask enums.
/// \~russian @brief Определяет расширенный класс перечисления.
///     Этот макрос генерирует класс, который оборачивает стандартное перечисление C++,
///     предоставляя дополнительные функции, такие как преобразование в/из std::string,
///     итерацию по значениям перечисления и конструирование из целочисленного или строкового представления.
///     Он автоматически создает внутреннее перечисление `TEnumInternal`, конструкторы,
///     операторы преобразования, статический метод `getEnums()` для получения всех значений перечисления,
///     статические методы `convert()` для преобразования строка/перечисление и
///     `convertToFlagsString()` для битовых масок перечислений.
/// \~english @param TEnum The name of the enum class to be generated.
/// \~russian @param TEnum Имя генерируемого класса перечисления.
/// \~english @param ... A variadic argument list representing the enumerators of the enum
///     (e.g., `VAL1, VAL2, VAL3=10, VAL4`). An additional `UNKNOWN` enumerator with value `INT_MIN` is automatically added.
/// \~russian @param ... Список аргументов переменной длины, представляющий перечислители
///     (например, `VAL1, VAL2, VAL3=10, VAL4`). Дополнительный перечислитель `UNKNOWN` со значением `INT_MIN` добавляется автоматически.
#define DEFINE_ENUM(TEnum, ...) \
class TEnum { \
public: \
    DISABLE_C26827 \
    enum TEnumInternal { \
        __VA_ARGS__, \
        UNKNOWN = INT_MIN \
    }; \
    ENABLE_C26827 \
    TEnum(): m_value(TEnumInternal::UNKNOWN) {} \
    TEnum(TEnum const &e) = default; \
    TEnum(TEnumInternal const &e): m_value(e) {} \
    TEnum(int const &value): m_value(static_cast<TEnumInternal>(value)) {} \
    TEnum(std::string const &s): m_value(UNKNOWN) { \
        for (auto const &i: map_enum_string) \
            if (iridium::lowerCase(i.second) == iridium::lowerCase(s)) { \
                m_value = i.first; \
                break; \
            } \
    } \
   ~TEnum() = default; \
    operator std::string() const { \
        try { \
            return map_enum_string.at(m_value); \
        } catch (...) { \
            return "UNKNOWN"; \
        } \
    } \
    operator TEnumInternal() const { \
        return m_value; \
    } \
private: \
    class Enums { \
    public: \
        Enums(TEnum::TEnumInternal const &e) { \
            index = e; \
            enums.push_back(TEnum(e)); \
        } \
        Enums(int const &e) { \
            index = e; \
            enums.push_back(TEnum(static_cast<TEnum::TEnumInternal>(e))); \
        } \
        Enums() { \
            index++; \
            enums.push_back(TEnum(static_cast<TEnum::TEnumInternal>(index))); \
        } \
       ~Enums() = default; \
        static std::list<TEnum> enums; \
        static int index; \
    }; \
    TEnumInternal m_value; \
    static std::map<TEnumInternal, std::string> const map_enum_string; \
    static std::map<TEnumInternal, std::string> const generateMap() { \
        auto m = std::map<TEnumInternal, std::string>(); \
        Enums UNUSED __VA_ARGS__; \
        auto i = Enums::enums.begin(); \
        for (auto const &arg:  iridium::split(#__VA_ARGS__, ",")) { \
            std::string name = iridium::trim(iridium::split(arg, "=").front()); \
            if (m.find(*i) == m.end()) \
                (m)[*i++] = name; \
            else \
                throw std::runtime_error(std::string(#TEnum) + " map key collision " + (m)[*i] + " and " + name); \
        } \
        return std::forward<std::map<TEnumInternal, std::string> const>(m); \
    } \
public: \
    static std::list<TEnum> const &getEnums() { \
        return Enums::enums; \
    } \
    static TEnum convert(std::string const &s) { \
        TEnum e(s); \
        if (e == UNKNOWN) { \
            auto message = "convert enum string '" + s + "' to enum " + std::string(#TEnum) + " error: valid enum keys:"; \
            for (auto const &i: getEnums()) \
                message += std::string(" ") + static_cast<std::string>(TEnum(i)) + " = " + iridium::convertion::convert<std::string>(static_cast<int>(i)); \
            throw std::runtime_error(message); \
        } else \
            return e; \
    } \
    static std::string convert(TEnum const &e) { \
        return static_cast<std::string>(e); \
    } \
    static std::string convert(TEnum::TEnumInternal const &e) { \
        return convert(TEnum(e)); \
    } \
    static std::string convert(std::list<TEnum::TEnumInternal> const &enums_) { \
        std::string result; \
        for (auto const &e: enums_) \
            result += convert(e) + ", "; \
        if (result.size() > 2) \
            result.resize(result.size() - 2); \
        return result; \
    } \
    std::string convertToFlagsString() const { \
        std::string result; \
        for (auto const &f: TEnum::getEnums()) \
            if (m_value & f) \
                result += convert(f) + " "; \
        return result; \
    } \
};
//DEFINE_CONVERT(TEnum, std::string)
//DEFINE_CONVERT(std::string, TEnum)
//DEFINE_CONVERT(std::string, TEnum::TEnumInternal)


#define DEFINE_ENUM_HASH(TEnum) \
namespace std { \
    template<> \
    struct hash<TEnum> { \
        std::size_t operator()(TEnum const &e) const noexcept { \
            return std::hash<int>()(static_cast<int>(static_cast<TEnum::TEnumInternal>(e))); \
        } \
    }; \
}


/// \~english @brief Implements the static members and conversion functions for an enhanced enum class.
///     This macro provides the definitions for the static members (like the map for string conversion
///     and the list of enum values) and helper functions that were declared by the `DEFINE_ENUM` macro.
///     It should typically be placed in a .cpp file to avoid multiple definitions.
/// \~russian @brief Реализует статические члены и функции преобразования для расширенного класса перечисления.
///     Этот макрос предоставляет определения для статических членов (таких как карта для преобразования строк
///     и список значений перечисления) и вспомогательных функций, которые были объявлены макросом `DEFINE_ENUM`.
///     Обычно его следует размещать в файле .cpp, чтобы избежать множественных определений.
/// \~english @param TEnum The name of the enum class that was previously defined using `DEFINE_ENUM`.
/// \~russian @param TEnum Имя класса перечисления, который был ранее определен с помощью `DEFINE_ENUM`.
/// \~english @note This macro should be used outside of any namespace.
/// \~russian @note Этот макрос должен использоваться вне каких-либо пространств имен.
#define IMPLEMENT_ENUM(TEnum) \
std::list<TEnum> TEnum::Enums::enums = std::list<TEnum>(); \
int TEnum::Enums::index = -1; \
std::map<TEnum::TEnumInternal, std::string> const TEnum::map_enum_string(TEnum::generateMap()); \
IMPLEMENT_CONVERT(TEnum, std::string, TEnum::convert) \
IMPLEMENT_CONVERT(std::string, TEnum::TEnumInternal, TEnum::convert) \
IMPLEMENT_CONVERT(std::string, TEnum, TEnum::convert) \
IMPLEMENT_CONVERT(std::string, std::list<TEnum::TEnumInternal>, TEnum::convert)


#endif // HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C


/*
static std::string convert(TEnum const &e) { \
    auto result = static_cast<std::string>(e); \
    if (result == "UNKNOWN") { \
        auto message = "convert enum " + std::string(#TEnum) + " int " + iridium::convertion::convert<std::string>(static_cast<int>(static_cast<TEnumInternal>(e))) + " to string error: valid enum keys is"; \
        for (auto const &i: getEnums()) \
            message += " " + static_cast<std::string>(TEnum(i)) + " = " + iridium::convertion::convert<std::string>(static_cast<int>(i)); \
        throw std::runtime_error(message); \
    } \
    return result; \
} \
*/
