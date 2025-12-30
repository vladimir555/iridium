// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
#define HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709


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


struct config {
    static thread_local int double_precission;
};


template<>
struct TConvert<std::string, std::chrono::system_clock::time_point> {
    static std::string convert(std::chrono::system_clock::time_point const &value, bool const &is_gmt_time);
    static std::string convert(std::chrono::system_clock::time_point const &value);
};


template<>
struct TConvert<std::string, std::chrono::hours> {
    static std::string convert(std::chrono::hours const &value);
};


template<>
struct TConvert<std::string, std::chrono::minutes> {
    static std::string convert(std::chrono::minutes const &value);
};


template<>
struct TConvert<std::string, std::chrono::seconds> {
    static std::string convert(std::chrono::seconds const &value);
};


template<>
struct TConvert<std::string, std::chrono::milliseconds> {
    static std::string convert(std::chrono::milliseconds const &value);
};


template<>
struct TConvert<std::string, std::chrono::microseconds> {
    static std::string convert(std::chrono::microseconds const &value);
};


template<>
struct TConvert<std::string, std::chrono::nanoseconds> {
    static std::string convert(std::chrono::nanoseconds const &value);
};


template<>
struct TConvert<std::string, bool> {
    static std::string convert(bool const &value);
};


template<>
struct TConvert<std::string, int64_t> {
    static std::string convert(int64_t const &value, uint8_t const &base);
    static std::string convert(int64_t const &value);
};


template<>
struct TConvert<std::string, uint64_t> {
    static std::string convert(uint64_t const &value, uint8_t const &base);
    static std::string convert(uint64_t const &value);
};


template<>
struct TConvert<std::string, int32_t> {
    static std::string convert(int32_t const &value, uint8_t const &base);
    static std::string convert(int32_t const &value);
};


template<>
struct TConvert<std::string, uint32_t> {
    static std::string convert(uint32_t const &value, uint8_t const &base);
    static std::string convert(uint32_t const &value);
};


template<>
struct TConvert<std::string, int16_t> {
    static std::string convert(int16_t const &value, uint8_t const &base);
    static std::string convert(int16_t const &value);
};


template<>
struct TConvert<std::string, uint16_t> {
    static std::string convert(uint16_t const &value, uint8_t const &base);
    static std::string convert(uint16_t const &value);
};


template<>
struct TConvert<std::string, int8_t> {
    static std::string convert(int8_t const &value, uint8_t const &base);
    static std::string convert(int8_t const &value);
};


template<>
struct TConvert<std::string, uint8_t> {
    static std::string convert(uint8_t const &value, uint8_t const &base);
    static std::string convert(uint8_t const &value);
};


template<>
struct TConvert<std::string, double> {
    static std::string convert(double const &value, uint8_t const &precision);
    static std::string convert(double const &value);
};


template<>
struct TConvert<std::string, float> {
    static std::string convert(float const &value, uint8_t const &precision);
    static std::string convert(float const &value);
};


template<>
struct TConvert<std::string, std::thread::id> {
    static std::string convert(std::thread::id const &value);
};


template<>
struct TConvert<std::string, std::exception> {
    static std::string convert(std::exception const &value);
};


template<>
struct TConvert<std::string, std::nested_exception> {
    static std::string convert(std::nested_exception const &value);
};


template<>
struct TConvert<std::chrono::system_clock::time_point, std::string> {
    static std::chrono::system_clock::time_point convert(std::string const &value);
};


template<>
struct TConvert<bool, std::string> {
    static bool convert(std::string const &value);
};


template<>
struct TConvert<int32_t, std::string> {
    static int32_t convert(std::string const &value);
};


template<>
struct TConvert<int64_t, std::string> {
    static int64_t convert(std::string const &value);
};


template<>
struct TConvert<uint32_t, std::string> {
    static uint32_t convert(std::string const &value);
};


template<>
struct TConvert<uint16_t, std::string> {
    static uint16_t convert(std::string const &value);
};


template<>
struct TConvert<uint8_t, std::string> {
    static uint8_t convert(std::string const &value);
};


// todo: string -> uint with base
template<>
struct TConvert<uint64_t, std::string> {
    static uint64_t convert(std::string const &value);
};


template<>
struct TConvert<double, std::string> {
    static double convert(std::string const &value);
};


template<>
struct TConvert<float, std::string> {
    static float convert(std::string const &value);
};


template<>
struct TConvert<int, int> {
    static int convert(int const &value);
};


#ifdef __OHOS__
template<>
struct TConvert<std::string, long long> {
    static std::string convert(long long const &value);
};
#endif // __OHOS__


template<>
struct TConvert<std::string, std::wstring> {
    static std::string convert(std::wstring const &value);
};


template<>
struct TConvert<std::wstring, std::string> {
    static std::wstring convert(std::string const &value);
};


template<>
struct TConvert<std::string, std::u16string> {
    static std::string convert(std::u16string const &value);
};


template<>
struct TConvert<std::string, std::u32string> {
    static std::string convert(std::u32string const &value);
};


template<>
struct TConvert<std::u16string, std::string> {
    static std::u16string convert(std::string const &value);
};


template<>
struct TConvert<std::u32string, std::string> {
    static std::u32string convert(std::string const &value);
};


template<>
struct TConvert<std::string, std::string> {
    static std::string convert(std::string const &value);
};


} // iridium::convertion::implementation


#endif // HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
