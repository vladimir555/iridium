// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
#define HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709


/// \~english @file
/// @brief Declares specializations of the `TConvert` template for various types.
/// \~russian @file
/// @brief Объявляет специализации шаблона `TConvert` для различных типов.


#include "common.h"


#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <stdexcept>
#include <sstream>


// ----- implementation


#include "iridium/platform.h"
#include PLATFORM_HEADER(convert.h)


namespace iridium::convertion::implementation {


/// \~english @brief Configuration for the conversion framework.
/// \~russian @brief Конфигурация для фреймворка преобразования.
struct config {
    /// \~english @brief The precision to use for double to string conversions.
    /// \~russian @brief Точность, используемая для преобразования double в строку.
    static std::atomic<int> double_precission;
};


/// \~english @brief Specialization for converting a `std::chrono::system_clock::time_point` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::system_clock::time_point` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::system_clock::time_point> {
    static std::string convert(std::chrono::system_clock::time_point const &value, bool const &is_gmt_time);
    static std::string convert(std::chrono::system_clock::time_point const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::hours` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::hours` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::hours> {
    static std::string convert(std::chrono::hours const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::minutes` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::minutes` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::minutes> {
    static std::string convert(std::chrono::minutes const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::seconds` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::seconds` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::seconds> {
    static std::string convert(std::chrono::seconds const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::milliseconds` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::milliseconds` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::milliseconds> {
    static std::string convert(std::chrono::milliseconds const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::microseconds` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::microseconds` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::microseconds> {
    static std::string convert(std::chrono::microseconds const &value);
};


/// \~english @brief Specialization for converting a `std::chrono::nanoseconds` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::chrono::nanoseconds` в `std::string`.
template<>
struct TConvert<std::string, std::chrono::nanoseconds> {
    static std::string convert(std::chrono::nanoseconds const &value);
};


/// \~english @brief Specialization for converting a `bool` to a `std::string`.
/// \~russian @brief Специализация для преобразования `bool` в `std::string`.
template<>
struct TConvert<std::string, bool> {
    static std::string convert(bool const &value);
};


/// \~english @brief Specialization for converting an `int64_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `int64_t` в `std::string`.
template<>
struct TConvert<std::string, int64_t> {
    static std::string convert(int64_t const &value, uint8_t const &base);
    static std::string convert(int64_t const &value);
};


/// \~english @brief Specialization for converting a `uint64_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `uint64_t` в `std::string`.
template<>
struct TConvert<std::string, uint64_t> {
    static std::string convert(uint64_t const &value, uint8_t const &base);
    static std::string convert(uint64_t const &value);
};


/// \~english @brief Specialization for converting an `int32_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `int32_t` в `std::string`.
template<>
struct TConvert<std::string, int32_t> {
    static std::string convert(int32_t const &value, uint8_t const &base);
    static std::string convert(int32_t const &value);
};


/// \~english @brief Specialization for converting a `uint32_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `uint32_t` в `std::string`.
template<>
struct TConvert<std::string, uint32_t> {
    static std::string convert(uint32_t const &value, uint8_t const &base);
    static std::string convert(uint32_t const &value);
};


/// \~english @brief Specialization for converting an `int16_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `int16_t` в `std::string`.
template<>
struct TConvert<std::string, int16_t> {
    static std::string convert(int16_t const &value, uint8_t const &base);
    static std::string convert(int16_t const &value);
};


/// \~english @brief Specialization for converting a `uint16_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `uint16_t` в `std::string`.
template<>
struct TConvert<std::string, uint16_t> {
    static std::string convert(uint16_t const &value, uint8_t const &base);
    static std::string convert(uint16_t const &value);
};


/// \~english @brief Specialization for converting an `int8_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `int8_t` в `std::string`.
template<>
struct TConvert<std::string, int8_t> {
    static std::string convert(int8_t const &value, uint8_t const &base);
    static std::string convert(int8_t const &value);
};


/// \~english @brief Specialization for converting a `uint8_t` to a `std::string`.
/// \~russian @brief Специализация для преобразования `uint8_t` в `std::string`.
template<>
struct TConvert<std::string, uint8_t> {
    static std::string convert(uint8_t const &value, uint8_t const &base);
    static std::string convert(uint8_t const &value);
};


/// \~english @brief Specialization for converting a `double` to a `std::string`.
/// \~russian @brief Специализация для преобразования `double` в `std::string`.
template<>
struct TConvert<std::string, double> {
    static std::string convert(double const &value, uint8_t const &precision);
    static std::string convert(double const &value);
};


/// \~english @brief Specialization for converting a `float` to a `std::string`.
/// \~russian @brief Специализация для преобразования `float` в `std::string`.
template<>
struct TConvert<std::string, float> {
    static std::string convert(float const &value, uint8_t const &precision);
    static std::string convert(float const &value);
};


/// \~english @brief Specialization for converting a `std::thread::id` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::thread::id` в `std::string`.
template<>
struct TConvert<std::string, std::thread::id> {
    static std::string convert(std::thread::id const &value);
};


/// \~english @brief Specialization for converting a `std::exception` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::exception` в `std::string`.
template<>
struct TConvert<std::string, std::exception> {
    static std::string convert(std::exception const &value);
};


/// \~english @brief Specialization for converting a `std::nested_exception` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::nested_exception` в `std::string`.
template<>
struct TConvert<std::string, std::nested_exception> {
    static std::string convert(std::nested_exception const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `std::chrono::system_clock::time_point`.
/// \~russian @brief Специализация для преобразования `std::string` в `std::chrono::system_clock::time_point`.
template<>
struct TConvert<std::chrono::system_clock::time_point, std::string> {
    static std::chrono::system_clock::time_point convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `bool`.
/// \~russian @brief Специализация для преобразования `std::string` в `bool`.
template<>
struct TConvert<bool, std::string> {
    static bool convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to an `int32_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `int32_t`.
template<>
struct TConvert<int32_t, std::string> {
    static int32_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to an `int64_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `int64_t`.
template<>
struct TConvert<int64_t, std::string> {
    static int64_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `uint32_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `uint32_t`.
template<>
struct TConvert<uint32_t, std::string> {
    static uint32_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `uint16_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `uint16_t`.
template<>
struct TConvert<uint16_t, std::string> {
    static uint16_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `uint8_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `uint8_t`.
template<>
struct TConvert<uint8_t, std::string> {
    static uint8_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `uint64_t`.
/// \~russian @brief Специализация для преобразования `std::string` в `uint64_t`.
template<>
struct TConvert<uint64_t, std::string> {
    static uint64_t convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `double`.
/// \~russian @brief Специализация для преобразования `std::string` в `double`.
template<>
struct TConvert<double, std::string> {
    static double convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `float`.
/// \~russian @brief Специализация для преобразования `std::string` в `float`.
template<>
struct TConvert<float, std::string> {
    static float convert(std::string const &value);
};


/// \~english @brief Specialization for converting an `int` to an `int`.
/// \~russian @brief Специализация для преобразования `int` в `int`.
template<>
struct TConvert<int, int> {
    static int convert(int const &value);
};


/// \~english @brief Specialization for converting a `std::wstring` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::wstring` в `std::string`.
template<>
struct TConvert<std::string, std::wstring> {
    static std::string convert(std::wstring const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `std::wstring`.
/// \~russian @brief Специализация для преобразования `std::string` в `std::wstring`.
template<>
struct TConvert<std::wstring, std::string> {
    static std::wstring convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::u16string` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::u16string` в `std::string`.
template<>
struct TConvert<std::string, std::u16string> {
    static std::string convert(std::u16string const &value);
};


/// \~english @brief Specialization for converting a `std::u32string` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::u32string` в `std::string`.
template<>
struct TConvert<std::string, std::u32string> {
    static std::string convert(std::u32string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `std::u16string`.
/// \~russian @brief Специализация для преобразования `std::string` в `std::u16string`.
template<>
struct TConvert<std::u16string, std::string> {
    static std::u16string convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `std::u32string`.
/// \~russian @brief Специализация для преобразования `std::string` в `std::u32string`.
template<>
struct TConvert<std::u32string, std::string> {
    static std::u32string convert(std::string const &value);
};


/// \~english @brief Specialization for converting a `std::string` to a `std::string`.
/// \~russian @brief Специализация для преобразования `std::string` в `std::string`.
template<>
struct TConvert<std::string, std::string> {
    static std::string convert(std::string const &value);
};


} // namespace iridium::convertion::implementation


#endif // HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
