#ifndef HEADER_COMMON_BAC2AFE8_4236_4301_A650_E60876F56E15
#define HEADER_COMMON_BAC2AFE8_4236_4301_A650_E60876F56E15


#include <type_traits>
#include <string>
#include <sstream>
#include <memory>


namespace iridium::convertion::implementation {


size_t const int_to_string_buffer_size = 64;


template<
    typename TResult,
    typename TValue,
    typename TIsEnabled = void,
    bool is_throwable = false
>
struct TConvert;


namespace detail {


template<typename T>
struct TIsInSTDNamespaceImplementation;


template<typename T>
auto testIsSTD(int) -> decltype(TIsInSTDNamespaceImplementation<T>{}, std::true_type{});


template<typename>
auto testIsSTD(...) -> std::false_type;


template<typename T>
struct TIsSTLType: decltype(detail::testIsSTD<T>(0)) {};


template<typename, typename = void>
struct TIsSTLSequentialContainer: std::false_type {};


template<typename T>
struct TIsSTLSequentialContainer<
    T,
    std::enable_if_t<
        std::is_class_v<T> &&
        std::is_same_v<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())> >
>:  std::true_type {};


template<typename, typename = void>
struct TIsSTLAssociativeContainer: std::false_type {};


template<typename T>
struct TIsSTLAssociativeContainer<
    T,
    std::void_t<
        typename T::key_type,
        typename T::mapped_type > >
{
    static constexpr bool value = TIsSTLType<T>::value;
};


template<typename, typename, typename = void>
struct THasConvert : std::false_type {};


template<typename TTo, typename TFrom>
struct THasConvert<
    TTo,
    TFrom,
    std::void_t<
        decltype(TConvert<TTo, TFrom>::convert(std::declval<TFrom const &>()))
    >
>:  std::true_type {};


} // detail


// ----- interface


template<typename TResult, typename TValue, typename TIsEnabled, bool is_throwable>
struct TConvertPolicy {
    static TResult convert(TValue const &value) {
        if constexpr (detail::THasConvert<TResult, TValue>::value) {
            return TConvert<TResult, TValue>::convert(value);
        } else {
            if constexpr (is_throwable) {
                throw std::runtime_error(
                    std::string("conversion error: no specialization exists for TValue(") +
                    typeid(TValue).name() + "), TResult(" + typeid(TResult).name() + ")"
                );
            } else {
                static_assert(sizeof(TResult) == 0,
                    "TConvert specialization is missing for the given TResult and TValue types");
            }
        }
    }

    template<typename TFormat>
    static TResult convert(TValue const &value, TFormat const &format) {
        if constexpr (detail::THasConvert<TResult, TValue>::value) {
            return TConvert<TResult, TValue>::convert(value, format);
        } else {
            if constexpr (is_throwable) {
                throw std::runtime_error(
                    std::string("conversion error: no specialization exists for TValue(") +
                    typeid(TValue).name() + "), TResult(" + typeid(TResult).name() + "), TFormat(" + typeid(TFormat).name() + ")"
                );
            } else {
                static_assert(sizeof(TResult) == 0,
                    "TConvert specialization is missing for the given TResult, TValue, TFormat types");
            }
        }
    }
};


template<typename TValue>
struct TConvert<std::string, std::shared_ptr<TValue> > {
    static std::string convert(std::shared_ptr<TValue> const &ptr) {
        if (ptr) {
            return TConvert<std::string, TValue>::convert(*ptr);
        } else {
            return "null";
        }
    }
};


template<typename TValue>
struct TConvert<std::string, std::atomic<TValue> > {
    static std::string convert(std::atomic<TValue> const &value) {
        return TConvert<std::string, TValue>::convert(value.load());
    }
};


template<typename TContainer, bool is_throwable>
struct TConvert<
    std::string,
    TContainer,
    std::enable_if_t<
        detail::TIsSTLSequentialContainer <TContainer>::value &&
       !detail::TIsSTLAssociativeContainer<TContainer>::value>, is_throwable >
{
    static std::string convert(TContainer const &container) {
        std::string result;
        result.reserve(container.size() * 8 + 4);
        result = "[ ";
        bool first = true;

        for (auto const &item: container) {
            if (!first) {
                result += ", ";
            } else {
                first = false;
            }
            result += TConvert<std::string, std::decay_t<decltype(item)>>::convert(item);
        }

        result += " ]";

        return result;
    }
};


template<typename TContainer, bool is_throwable>
struct TConvert<
    std::string,
    TContainer,
    std::enable_if_t<
       !detail::TIsSTLSequentialContainer <TContainer>::value &&
        detail::TIsSTLAssociativeContainer<TContainer>::value>, is_throwable >
{
    static std::string convert(TContainer const &container) {
        std::string result;
        result.reserve(container.size() * 8 + 4);
        result = "[ ";
        bool first = true;

        for (auto const &pair: container) {
            if (!first) {
                result += ", ";
            } else {
                first = false;
            }
            result += TConvert<std::string, typename TContainer::key_type>::convert(pair.first);
            result += ": ";
            result += TConvert<std::string, typename TContainer::mapped_type>::convert(pair.second);
        }

        result += " ]";

        return result;
    }
};


} // iridium::convertion::implementation


#endif // HEADER_COMMON_BAC2AFE8_4236_4301_A650_E60876F56E15
