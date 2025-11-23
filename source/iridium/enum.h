// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
#define HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C


/// \~english @file
/// @brief Provides macros for defining and implementing type-safe enums with reflection capabilities.
/// \~russian @file
/// @brief Предоставляет макросы для определения и реализации типобезопасных перечислений с возможностями рефлексии.


#include "iridium/strings.h"
#include "iridium/convertion/convert.h"

#include <list>
#include <map>
#include <iostream>
#include <limits.h>


#ifdef _MSC_VER
/// \~english @def UNUSED
/// @brief A macro to mark a variable as unused, suppressing compiler warnings. Specific to MSVC.
/// \~russian @def UNUSED
/// @brief Макрос для пометки переменной как неиспользуемой, подавляющий предупреждения компилятора. Специфично для MSVC.
#define UNUSED
#define DISABLE_C26827 __pragma(warning(push)) __pragma(warning(suppress: 26827))
#define ENABLE_C26827  __pragma(warning(pop))
#else
/// \~english @def UNUSED
/// @brief A macro to mark a variable as unused, suppressing compiler warnings. Uses `__attribute__((unused))` for GCC/Clang.
/// \~russian @def UNUSED
/// @brief Макрос для пометки переменной как неиспользуемой, подавляющий предупреждения компилятора. Использует `__attribute__((unused))` для GCC/Clang.
#define UNUSED __attribute__((unused))
#define DISABLE_C26827
#define ENABLE_C26827
#endif // _MSC_VER


/// \~english @def DEFINE_ENUM(TEnum, ...)
/// @brief Defines a class that encapsulates a scoped enum, providing type safety and utility functions.
/// This macro generates a class `TEnum` with an internal enum, providing constructors from the underlying type, integers,
/// and strings. It also includes conversions to `std::string` and the internal enum type, along with static methods for
/// introspection (e.g., getting all enum values) and conversion.
/// \~russian @def DEFINE_ENUM(TEnum, ...)
/// @brief Определяет класс, который инкапсулирует перечисление с областью видимости, обеспечивая строгую типизацию и вспомогательные функции.
/// Этот макрос генерирует класс `TEnum` с внутренним перечислением, предоставляя конструкторы из базового типа, целых чисел
/// и строк. Он также включает преобразования в `std::string` и внутренний тип перечисления, а также статические методы для
/// интроспекции (например, получение всех значений перечисления) и преобразования.
/// \~english @param TEnum The name of the enum class to be defined.
/// \~russian @param TEnum Имя класса перечисления, которое будет определено.
/// \~english @param ... A comma-separated list of enumerator definitions (e.g., `VALUE1, VALUE2, VALUE3 = 10`).
/// \~russian @param ... Список определений перечислителей, разделенных запятыми (например, `VALUE1, VALUE2, VALUE3 = 10`).
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
        for (auto const &i: map_enum_string) { \
            if (iridium::lowerCase(i.second) == iridium::lowerCase(s)) { \
                m_value = i.first; \
                break; \
            } \
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
    static std::string convert(std::list<TEnum> const &enums_) { \
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


/// \~english @def DEFINE_ENUM_CONVERT(TEnum)
/// @brief Specializes the `iridium::convertion` framework and `std::hash` for an enum created with `DEFINE_ENUM`.
/// This macro provides template specializations for `TConvert` to allow seamless conversion between the enum's internal
/// type and `std::string`. It also specializes `std::hash<TEnum>`, enabling enum instances to be used in unordered containers.
/// \~russian @def DEFINE_ENUM_CONVERT(TEnum)
/// @brief Специализирует фреймворк `iridium::convertion` и `std::hash` для перечисления, созданного с помощью `DEFINE_ENUM`.
/// Этот макрос предоставляет специализации шаблона `TConvert` для безшовного преобразования между внутренним
/// типом перечисления и `std::string`. Он также специализирует `std::hash<TEnum>`, позволяя использовать экземпляры перечисления в неупорядоченных контейнерах.
/// \~english @param TEnum The enum class for which to define the conversion and hashing specializations.
/// \~russian @param TEnum Класс перечисления, для которого определяются специализации преобразования и хеширования.
#define DEFINE_ENUM_CONVERT(TEnum) \
template<> \
struct iridium::convertion::implementation::TConvert<std::string, TEnum::TEnumInternal> { \
    static std::string convert(TEnum::TEnumInternal const &e) { \
        return TEnum::convert(TEnum(e)); \
    } \
}; \
template<> \
struct iridium::convertion::implementation::TConvert<TEnum::TEnumInternal, std::string> { \
    static TEnum::TEnumInternal convert(std::string const &e) { \
        return TEnum::convert(e); \
    } \
}; \
namespace std { \
template<> \
struct hash<TEnum> { \
    size_t operator()(TEnum const &value) const noexcept { \
        return std::hash<int>()(static_cast<int>(static_cast<TEnum::TEnumInternal>(value))); \
    } \
}; \
}


/// \~english @def IMPLEMENT_ENUM(TEnum)
/// @brief Implements the static members of an enum class defined with `DEFINE_ENUM`.
/// This macro must be placed in a source file (.cpp) to define the static member variables
/// that `DEFINE_ENUM` declares, which manage the list of enumerators and their string representations.
/// \~russian @def IMPLEMENT_ENUM(TEnum)
/// @brief Реализует статические члены класса перечисления, определенного с помощью `DEFINE_ENUM`.
/// Этот макрос должен быть помещен в файл исходного кода (.cpp) для определения статических
/// переменных-членов, объявленных `DEFINE_ENUM`, которые управляют списком перечислителей и их строковыми представлениями.
/// \~english @param TEnum The enum class for which to implement the static members.
/// \~russian @param TEnum Класс перечисления, для которого реализуются статические члены.
#define IMPLEMENT_ENUM(TEnum) \
std::list<TEnum> TEnum::Enums::enums = std::list<TEnum>(); \
int TEnum::Enums::index = -1; \
std::map<TEnum::TEnumInternal, std::string> const TEnum::map_enum_string(TEnum::generateMap());


namespace iridium::convertion::implementation {


namespace detail {


/// \~english @brief A trait to check if a type `T` is an enum defined with `DEFINE_ENUM`.
/// \~russian @brief типаж для проверки, является ли тип `T` перечислением, определенным с помощью `DEFINE_ENUM`.
template<typename T, typename = void>
struct TIsIridiumEnum: std::false_type {};
/// \~english @brief A trait to check if a type `T` is an enum defined with `DEFINE_ENUM`. Specialization for when it is.
/// \~russian @brief типаж для проверки, является ли тип `T` перечислением, определенным с помощью `DEFINE_ENUM`. Специализация для случая, когда является.
template<typename T>
struct TIsIridiumEnum<T, std::void_t<typename T::TEnumInternal> > : std::true_type {};


} // namespace detail


template<typename TEnum>
struct TConvert<
    std::string,
    TEnum,
    std::enable_if_t<!std::is_enum_v<TEnum> && detail::TIsIridiumEnum<TEnum>::value> >
{
    static std::string convert(TEnum const &value) {
        return TEnum::convert(value);
    }
};


template<typename TEnum>
struct TConvert<
    TEnum,
    std::string,
    std::enable_if_t<detail::TIsIridiumEnum<TEnum>::value> >
{
    static TEnum convert(std::string const &value) {
        return TEnum::convert(value);
    }
};


} // namespace iridium::convertion::implementation


#endif // HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
