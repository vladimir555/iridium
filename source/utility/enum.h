#ifndef HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
#define HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C


#include "utility/strings.h"
#include "utility/convertion/convert.h"

#include <list>
#include <map>
#include <iostream>


#define DEFINE_ENUM(TEnum, ...) \
class TEnum { \
public: \
    enum TEnumInternal { \
        UNKNOWN = -1, \
        __VA_ARGS__ \
    }; \
    TEnum(TEnumInternal const &e): m_value(e) {} \
    TEnum(std::string const &s): m_value(UNKNOWN) { \
        for (auto &i : map_enum_string) \
            if (utility::lowerCase(i.second) == utility::lowerCase(s)) \
                m_value = i.first; \
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
    static std::map<TEnumInternal, std::string> const &generateMap() { \
        static std::map<TEnumInternal, std::string> m; \
        Enums __attribute__((unused)) __VA_ARGS__; \
        auto i = Enums::enums.begin(); \
        for (auto const &arg: utility::split(#__VA_ARGS__, ",")) { \
            std::string name  = utility::trim(utility::split(arg, "=")[0]); \
            if (m.find(*i) == m.end()) \
                m[*i++] = name; \
            else \
                throw std::runtime_error(std::string(#TEnum) + " map key collision " + m[*i] + " and " + name); \
        } \
        return m; \
    } \
public: \
    static std::list<TEnum> const &getEnums() { \
        return Enums::enums; \
    } \
    static TEnum convert(std::string const &s) { \
        TEnum e(s); \
        if (e == UNKNOWN) { \
            auto message = "convert enum string '" + s + "' to enum " + std::string(#TEnum) + " error: valid enum keys is"; \
            for (auto const &i: getEnums()) \
                message += " " + static_cast<std::string>(TEnum(i)) + " = " + utility::convertion::convert<std::string>(static_cast<int>(i)); \
            throw std::runtime_error(message); \
        } else \
            return e; \
    } \
    static std::string convert(TEnum const &e) { \
        auto result = static_cast<std::string>(e); \
        if (result == "UNKNOWN") { \
            auto message = "convert enum " + std::string(#TEnum) + " int " + utility::convertion::convert<std::string>(static_cast<int>(static_cast<TEnumInternal>(e))) + " to string error: valid enum keys is"; \
            for (auto const &i: getEnums()) \
                message += " " + static_cast<std::string>(TEnum(i)) + " = " + utility::convertion::convert<std::string>(static_cast<int>(i)); \
            throw std::runtime_error(message); \
        } \
        return result; \
    } \
    static std::string convert(TEnum::TEnumInternal const &e) { \
        return convert(TEnum(e)); \
    } \
};
//DEFINE_CONVERT(TEnum, std::string)
//DEFINE_CONVERT(std::string, TEnum)
//DEFINE_CONVERT(std::string, TEnum::TEnumInternal)


#define IMPLEMENT_ENUM(TEnum) \
std::list<TEnum> TEnum::Enums::enums = std::list<TEnum>(); \
int TEnum::Enums::index = TEnum::UNKNOWN; \
std::map<TEnum::TEnumInternal, std::string> const TEnum::map_enum_string(TEnum::generateMap()); \
IMPLEMENT_CONVERT(TEnum, std::string, TEnum::convert) \
IMPLEMENT_CONVERT(std::string, TEnum, TEnum::convert) \
IMPLEMENT_CONVERT(std::string, TEnum::TEnumInternal, TEnum::convert)


#endif // HEADER_ENUM_98631D5A_6E4E_47DF_B3BA_220D5292687C
