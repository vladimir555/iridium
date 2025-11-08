#ifndef HEADER_COMMON_BAC2AFE8_4236_4301_A650_E60876F56E15
#define HEADER_COMMON_BAC2AFE8_4236_4301_A650_E60876F56E15


#include <type_traits>
#include <string>
#include <sstream>
#include <memory>


namespace iridium::convertion::implementation {


size_t const int_to_string_buffer_size = 64;


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
>: std::true_type {};


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


} // detail


// ----- interface


/// \~english @brief Default conversion implementation that causes a compile-time or run-time error.
/// \~russian @brief Реализация преобразования по умолчанию, которая вызывает ошибку во время компиляции или выполнения.
template<typename TResult, typename TValue, typename TIsEnabled = void, bool is_throwable = false>
struct TConvert
{
    /// \~english @brief Default conversion function that is called when no specialization is found.
    /// \~russian @brief Функция преобразования по умолчанию, которая вызывается, если не найдена специализация.
    static TResult convert(TValue const&)
    {
        if constexpr (is_throwable)
        {
            throw std::runtime_error(
                std::string("conversion error: no specialization exists for TValue(") +
                typeid(TValue).name() + "), TResult(" + typeid(TResult).name() + ")"
            );
        }
        else
        {
            static_assert(
                sizeof(TResult) == 0 || sizeof(TValue) == 0,
                "TConvert specialization is missing for the given TResult and TValue types");
        }
        return {};
    }
    /// \~english @brief Default conversion function with format that is called when no specialization is found.
    /// \~russian @brief Функция преобразования по умолчанию с форматом, которая вызывается, если не найдена специализация.
    template<typename TFormat>
    static TResult convert(TValue const&, TFormat const&)
    {
        if constexpr (is_throwable)
        {
            throw std::runtime_error(
                std::string("conversion error: no specialization exists for TValue(") +
                typeid(TValue).name() + "), TResult(" + typeid(TResult).name() + ")"
            );
        }
        else
        {
            static_assert(
                sizeof(TResult) == 0 || sizeof(TValue) == 0 || sizeof(TFormat) == 0,
                "TConvert specialization is missing for the given TResult and TValue types");
        }
        return {};
    }
};

/// \~english @brief Specialization for converting a shared pointer to a string.
/// \~russian @brief Специализация для преобразования умного указателя в строку.
template<typename TValue>
struct TConvert<std::string, std::shared_ptr<TValue>>
{
    /// \~english @brief Converts a shared pointer to a string.
    /// \~russian @brief Преобразует умный указатель в строку.
    static std::string convert(std::shared_ptr<TValue> const& ptr)
    {
        if (ptr)
        {
            return TConvert<std::string, TValue>::convert(*ptr);
        }
        else
        {
            return "null";
        }
    }
};


template<typename TValue>
struct TConvert<std::string, std::atomic<TValue>>
{
    static std::string convert(std::atomic<TValue> const& value)
    {
        return TConvert<std::string, TValue>::convert(value.load());
    }
};


template<typename TContainer, bool is_throwable>
struct TConvert<
    std::string,
    TContainer,
    std::enable_if_t<
        detail::TIsSTLSequentialContainer<TContainer>::value &&
        !detail::TIsSTLAssociativeContainer<TContainer>::value>,
    is_throwable>
{
    static std::string convert(TContainer const& container)
    {
        std::string result;
        result.reserve(container.size() * 8 + 4);
        result = "[ ";
        bool first = true;

        for (auto const& item : container)
        {
            if (!first)
            {
                result += ", ";
            }
            else
            {
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
        !detail::TIsSTLSequentialContainer<TContainer>::value &&
        detail::TIsSTLAssociativeContainer<TContainer>::value>,
    is_throwable>
{
    static std::string convert(TContainer const& container)
    {
        std::string result;
        result.reserve(container.size() * 8 + 4);
        result = "[ ";
        bool first = true;

        for (auto const& pair : container)
        {
            if (!first)
            {
                result += ", ";
            }
            else
            {
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
