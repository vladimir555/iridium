// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
#define HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C


#include "iridium/strings.h"
#include "iridium/convertion/convert.h"

#include <map>
#include <list>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <stdexcept>
#include <functional>
#include <type_traits>


#ifdef _MSC_VER
#define UNUSED
#define DISABLE_C26827 __pragma(warning(push)) __pragma(warning(suppress: 26827))
#define ENABLE_C26827  __pragma(warning(pop))
#else
#define UNUSED __attribute__((unused))
#define DISABLE_C26827
#define ENABLE_C26827
#endif // _MSC_VER


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
    explicit TEnum(int const &value): m_value(static_cast<TEnumInternal>(value)) {} \
    TEnum(std::string const &s): m_value(UNKNOWN) { \
        for (auto const &i: getMap()) { \
            if (iridium::lowerCase(i.second) == iridium::lowerCase(s)) { \
                m_value = i.first; \
                break; \
            } \
        } \
    } \
   ~TEnum() = default; \
    operator std::string() const { \
        try { \
            return getMap().at(m_value); \
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
            enums().push_back(TEnum(e)); \
        } \
        Enums(int const &e) { \
            index = e; \
            enums().push_back(TEnum(static_cast<TEnum::TEnumInternal>(e))); \
        } \
        Enums() { \
            index++; \
            enums().push_back(TEnum(static_cast<TEnum::TEnumInternal>(index))); \
        } \
       ~Enums() = default; \
        static std::list<TEnum> &enums() { \
            static std::list<TEnum> instance; \
            return instance; \
        } \
        inline static int index = -1; \
    }; \
    TEnumInternal m_value; \
    static std::map<TEnumInternal, std::string> const &getMap() { \
        static const std::map<TEnumInternal, std::string> map = \
            [] () { \
                auto m = std::map<TEnumInternal, std::string>(); \
                Enums UNUSED __VA_ARGS__; \
                auto i = Enums::enums().begin(); \
                for (auto const &arg:  iridium::split(#__VA_ARGS__, ",")) { \
                    std::string name = iridium::trim(iridium::split(arg, "=").front()); \
                    if (m.find(*i) == m.end()) \
                        (m)[*i++] = name; \
                    else { \
                        auto error = std::string(#TEnum) + " map key collision " + (m)[*i] + " and " + name; \
                        std::fprintf(stderr, "FATAL: %s\n", error.c_str()); \
                        std::abort(); \
                    } \
                } \
                return m; \
            } (); \
        return map; \
    } \
public: \
    static std::list<TEnum> const &getEnums() { \
        return Enums::enums(); \
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
                result += convert(f) + " | "; \
        if (result.size() > 3) \
            result.resize(result.size() - 3); \
        else \
            result = "UNKNOWN"; \
        return result; \
    } \
};


// #define DEFINE_ENUM_CONVERT(TEnum)


// todo: C++20/23 rm macros
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


#define IMPLEMENT_ENUM(TEnum) \
_Pragma("message(\"WARNING: IMPLEMENT_ENUM is deprecated\")")


namespace iridium::convertion::implementation {


namespace detail {


template<typename T, typename = void>
struct TIsIridiumEnum: std::false_type {};
template<typename T>
struct TIsIridiumEnum<T, std::void_t<typename T::TEnumInternal> > : std::true_type {};


} // detail


template<typename TEnum>
struct TConvert<
    std::string,
    TEnum,
    std::enable_if_t<!std::is_enum_v<TEnum> && detail::TIsIridiumEnum<TEnum>::value> >
{
    static std::string convert(TEnum const &value, bool const &is_flags = false) {
        if (is_flags)
            return value.convertToFlagsString();
        else
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


} // iridium::convertion::implementation


#endif // HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
