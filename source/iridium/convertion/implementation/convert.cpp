#include "iridium/strings.h"

#include "convert.h"

#include <chrono>
#include <ratio>
#include <algorithm>
#include <codecvt>
#include <stdexcept>
#include <locale>
#include <sstream>
#include <cmath>



//using std::chrono::high_resolution_clock;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::stringstream;
using std::time_t;
using std::list;
using std::string;
using std::wstring;
using std::u16string;
using std::u32string;
using std::tm;
using std::runtime_error;
//using std::wstring_convert;
//using std::codecvt_utf8_utf16;
//using std::codecvt_utf8;
using std::strftime;
using std::transform;
using std::shared_ptr;


namespace {


// 2015-05-05T05:05:05
char   const time_format_unix[]             = "%Y-%m-%d %H:%M:%S";
char   const time_scan_format[]             = "%04d-%02d-%02d %02d:%02d:%02d.%03d";
size_t const time_scan_format_size          = 23;
size_t const double_to_string_buffer_size   = 512;
size_t const time_to_string_buffer_size     = 64;


} // unnamed


namespace iridium::convertion::implementation {


std::atomic<int> config::double_precission(5);


//template<>
//string TConvert::convert(high_resolution_clock::time_point const &value) {
//    auto value_ms   = duration_cast<milliseconds>(value.time_since_epoch()).count();
//    auto ms         = value_ms % 1000;
//    time_t t        = value_ms / 1000;
//    auto tm_        = platform::gmtime(&t);
//
//    if (tm_) {
//        char buffer[time_to_string_buffer_size];
//        strftime(buffer, time_to_string_buffer_size, time_format_unix.c_str(), tm_);
//        return string(buffer) + "." + rjust(convert<string>(ms), 3, '0'); // ----->
//    } else
//        throw runtime_error("convert time_t (gmtime) to string error"); // ----->
//}


// typedef std::chrono::duration<int, std::ratio_multiply< minutes::period, std::ratio<5> >::type> _5minutes;
// tt = floor<_5minutes>(t);


string TConvert<string, system_clock::time_point>::convert(
    system_clock::time_point const &value,
    bool const &is_gmt_time)
{
    auto  value_ms  = duration_cast<milliseconds>(value.time_since_epoch()).count();
    uint32_t    ms  = value_ms % 1000;
    time_t      t   = system_clock::to_time_t(value);
    struct tm   tm_ = {};

    if (is_gmt_time)
        platform::gmtime_r(&t, &tm_);
    else
        platform::localtime_r(&t, &tm_);

    char buffer[time_to_string_buffer_size];
    strftime(buffer, time_to_string_buffer_size, time_format_unix, &tm_);
    return string(buffer) + "." + rjust(TConvert<string, uint32_t>::convert(ms), 3, '0'); // ----->
}


string TConvert<string, system_clock::time_point>::convert(
    system_clock::time_point const &value)
{
    return TConvert<string, system_clock::time_point>::convert(value, true); // ----->
}


string TConvert<string, hours>::convert(hours const &value) {
    return TConvert<string, hours::rep>::convert(static_cast<uint32_t>(value.count())) + " hours"; // ----->
}


string TConvert<string, minutes>::convert(minutes const &value) {
    return TConvert<string, minutes::rep>::convert(static_cast<uint32_t>(value.count())) + " minutes";
}

string TConvert<string, seconds>::convert(seconds const &value) {
    return TConvert<string, seconds::rep>::convert(static_cast<uint32_t>(value.count())) + " seconds";
}

string TConvert<string, milliseconds>::convert(milliseconds const &value) {
    return TConvert<string, milliseconds::rep>::convert(static_cast<uint32_t>(value.count())) + " milliseconds";
}

string TConvert<string, microseconds>::convert(microseconds const &value) {
    return TConvert<string, microseconds::rep>::convert(static_cast<uint32_t>(value.count())) + " microseconds";
}

string TConvert<string, nanoseconds>::convert(nanoseconds const &value) {
    return TConvert<string, nanoseconds::rep>::convert(static_cast<uint32_t>(value.count())) + " nanoseconds";
}


string TConvert<string, bool>::convert(bool const &value) {
    return value ? "true" : "false";
}


string TConvert<string, int64_t>::convert(int64_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, int64_t>::convert(int64_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


string TConvert<string, uint64_t>::convert(uint64_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return upperCase(string(buffer));
}


string TConvert<string, uint64_t>::convert(uint64_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return upperCase(string(buffer));
}


string TConvert<string, int32_t>::convert(int32_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return upperCase(string(buffer));
}


string TConvert<string, int32_t>::convert(int32_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return upperCase(string(buffer));
}


string TConvert<string, uint32_t>::convert(uint32_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, uint32_t>::convert(uint32_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


string TConvert<string, int16_t>::convert(int16_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, int16_t>::convert(int16_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


string TConvert<string, uint16_t>::convert(uint16_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, uint16_t>::convert(uint16_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


string TConvert<string, int8_t>::convert(int8_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, int8_t>::convert(int8_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


string TConvert<string, uint8_t>::convert(uint8_t const &value, uint8_t const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return string(buffer);
}


string TConvert<string, uint8_t>::convert(uint8_t const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, 10);
    return string(buffer);
}


template<typename TValue>
string convertRealToString(TValue const &value, std::string const &format) {
    if (std::isnan(value))
        return "NAN"; // ----->

    char buffer[double_to_string_buffer_size];
    platform::snprintf(buffer, double_to_string_buffer_size, format.c_str(), value);
    return buffer; // ----->
}


string TConvert<string, double>::convert(double const &value, uint8_t const &precision) {
    return convertRealToString(value, "%0." + TConvert<string, uint8_t>::convert(precision) + "lf");
}


string TConvert<string, double>::convert(double const &value) {
    return TConvert<string, double>::convert(value, static_cast<uint8_t>(config::double_precission));
}


string TConvert<string, float>::convert(float const &value, uint8_t const &precision) {
    return convertRealToString(value, "%0." + TConvert<string, uint8_t>::convert(precision) + "f");
}


string TConvert<string, float>::convert(float const &value) {
    return TConvert<string, float>::convert(value, static_cast<uint8_t>(config::double_precission));
}


string TConvert<string, std::thread::id>::convert(std::thread::id const &value) {
    stringstream ss;
    ss << value;
    return ss.str();
}


inline string formatException(std::exception const &e) {
    std::string result = e.what();
    const auto* nested = dynamic_cast<std::nested_exception const *>(&e);

    if (nested) {
        try {
            nested->rethrow_nested();
        } catch (std::exception const &inner) {
            result += ": " + formatException(inner);
        } catch (...) {
            result += ": unknown exception";
        }
    }

    return result;
}


string TConvert<string, std::exception>::convert(std::exception const &e) {
    return formatException(e);
}


string TConvert<string, std::nested_exception>::convert(std::nested_exception const &e) {
    try {
        e.rethrow_nested();
    } catch (std::exception const &inner) {
        return formatException(inner);
    } catch (...) {
        return "unknown exception";
    }

    return "";
}


system_clock::time_point TConvert<system_clock::time_point, string>::convert(string const &value) {
    if (value.size() != time_scan_format_size)
        throw runtime_error("convert '" + value + "' to time_t error, wrong source string format"); // ----->

    struct std::tm  tm_ = {};
    int ms = 0;
    int result = platform::sscanf(value.c_str(), time_scan_format,
        &tm_.tm_year,
        &tm_.tm_mon,
        &tm_.tm_mday,
        &tm_.tm_hour,
        &tm_.tm_min,
        &tm_.tm_sec,
        &ms);

    tm_.tm_year -= 1900;
    tm_.tm_mon  -= 1;

    if (result == 7) {
        auto time = platform::mkgmtime(&tm_);

        if (time < 0)
            throw runtime_error("convert '" + value + "' to time_t error, mkgmtime error"); // ----->

        return system_clock::time_point(std::chrono::seconds(time)) + std::chrono::milliseconds(ms); // ----->
    }
    else
        throw runtime_error("convert '" + value + "' to time_t error, sscanf: wrong source string format"); // ----->
}


bool TConvert<bool, string>::convert(string const &value_) {
    string value = value_;

    transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (value == "true")
        return true;  // ----->
    if (value == "false")
        return false; // ----->

    throw runtime_error("convert '" + value_ + "' to bool error"); // ----->
}


int32_t TConvert<int32_t, string>::convert(string const &value) {
    for (auto const ch: value)
        if ((ch < '0' || ch > '9') && ch != '-')
            throw runtime_error("convert '" + value + "' to int32 error"); // ----->

    int32_t i = atoi(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convert '" + value + "' to int32 error"); // ----->
    } else {
        return i; // ----->
    }
}


int64_t TConvert<int64_t, string>::convert(string const &value) {
    for (auto const ch : value)
        if ((ch < '0' || ch > '9') && ch != '-')
            throw runtime_error("convert '" + value + "' to int64 error"); // ----->

    int64_t i = atoll(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convert '" + value + "' to int64 error"); // ----->
    } else {
        return i; // ----->
    }
}


uint32_t TConvert<uint32_t, string>::convert(string const &value) {
    // todo: int test
    auto result = TConvert<uint64_t, string>::convert(value);
    if (result > UINT32_MAX)
        throw runtime_error("convert '" + value + "' to uint32 error"); // ----->
    return static_cast<uint32_t>(result); // ----->
}


uint16_t TConvert<uint16_t, string>::convert(string const &value) {
    auto result = TConvert<uint64_t, string>::convert(value);
    if (result > UINT16_MAX)
        throw runtime_error("convert '" + value + "' to uint16 error"); // ----->
    return static_cast<uint16_t>(result); // ----->
}


uint8_t TConvert<uint8_t, string>::convert(string const &value) {
    auto result = TConvert<uint64_t, string>::convert(value);
    if (result > UINT8_MAX)
        throw runtime_error("convert '" + value + "' to uint8 error"); // ----->
    return static_cast<uint8_t>(result); // ----->
}


uint64_t TConvert<uint64_t, string>::convert(string const &value) {
    return TConvert<int64_t, string>::convert(value); // ----->
}


template<typename TResult>
TResult convertStringToReal(string const &value, std::string const &format) {
    if (value == "NAN")
        return std::numeric_limits<TResult>::quiet_NaN(); // ----->

    for (auto const &ch: value)
        if ((ch < '0' || ch > '9') && ch != '.' && ch != '-')
            throw runtime_error("convert '" + value + "' to double error"); // ----->

    auto d = ::atof(value.c_str());

    // "%lf", "%f"
    if (d == 0.0) {
        int result = platform::sscanf(value.c_str(), format.c_str(), &d);
        if (result == 1)
            return static_cast<TResult>(d); // ----->
        else
            throw runtime_error("convert '" + value + "' to double error"); // ----->
    } else
        return static_cast<TResult>(d); // ----->
}


double TConvert<double, string>::convert(string const &value) {
    return convertStringToReal<double>(value, "%lf");
}


float TConvert<float, string>::convert(string const &value) {
    return convertStringToReal<float>(value, "%f");
}


int TConvert<int, int>::convert(int const &value) {
    return value; // ----->
}


#if __cplusplus >= 201103L && __cplusplus < 201703L


string TConvert<string, wstring>::convert(wstring const &value) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(value); // ----->
}


template<>
wstring TConvert<wstring, string>::convert(string const &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(value); // ----->
}


#else // C++11 <= STL < C++14


template<typename TResult, typename TArg, size_t size>
TResult convertInternal(TArg const &) {
    static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4,
        "convertion from/to wstring wchar_t size not 2 or 4 is not implemented");
}


template<>
string convertInternal<string, wstring, 2>(wstring const &value) {
    return TConvert<string, u16string>::convert(u16string(value.begin(), value.end())); // ----->
}


template<>
wstring convertInternal<wstring, string, 2>(string const &value) {
    auto result = TConvert<u16string, string>::convert(value);
    return wstring(result.begin(), result.end());
}


#ifndef WINDOWS_PLATFORM


template<>
string convertInternal<string, wstring, 4>(wstring const &value) {
    return TConvert<string, u32string>::convert(u32string(value.begin(), value.end())); // ----->
}


template<>
wstring convertInternal<wstring, string, 4>(string const &value) {
    auto result = TConvert<u32string, string>::convert(value);
    return wstring(result.begin(), result.end());
}


#endif // !WINDOWS_PLATFORM


string TConvert<string, wstring>::convert(wstring const &value) {
    return convertInternal<string, wstring, sizeof(wchar_t)>(value); // ----->
}


wstring TConvert<wstring, string>::convert(string const &value) {
    return convertInternal<wstring, string, sizeof(wchar_t)>(value); // ----->
}


string TConvert<string, u16string>::convert(u16string const &value) {
    // todo: test, not complete errors handling
    std::string result;
    size_t      length = value.length();

    for (size_t i = 0; i < length; ++i) {
        uint32_t symbol = value[i];

        if (symbol >= 0xD800 && symbol <= 0xDBFF) {
            if (i + 1 < length) {
                uint32_t low = value[++i];
                if (low >= 0xDC00 && low <= 0xDFFF)
                    symbol = 0x10000 + ((symbol - 0xD800) << 10) + (low - 0xDC00);
                else
                    throw std::runtime_error(
                        "convert utf16 to utf8 error: invalid surrogate pair sequence"); // ----->
            } else
                throw std::runtime_error(
                    "convert utf16 to utf8 error: high surrogate without following low surrogate"); // ----->
        } else
        if (symbol >= 0xDC00 && symbol <= 0xDFFF)
            throw std::runtime_error(
                "convert utf16 to utf8 error: orphan low surrogate without preceding high surrogate"); // ----->

        if (symbol <= 0x7F) {
            result.push_back(static_cast<char>(symbol));
        } else if (symbol <= 0x7FF) {
            result.push_back(static_cast<char>(0xC0 | ((symbol >> 6)  & 0x1F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else if (symbol <= 0xFFFF) {
            result.push_back(static_cast<char>(0xE0 | ((symbol >> 12) & 0x0F)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 6)  & 0x3F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else if (symbol <= 0x10FFFF) {
            result.push_back(static_cast<char>(0xF0 | ((symbol >> 18) & 0x07)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 12) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 6)  & 0x3F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else
            throw std::invalid_argument("convertion utf16 to utf8 error: invalid code point on position " +
                TConvert<string, uint64_t>::convert(i)); // ----->
    }
    return result; // ----->
}


string TConvert<string, u32string>::convert(u32string const &value) {
    // todo: test, not complete errors handling
    std::string result;

    for (char32_t const &symbol: value) {
        if (symbol <= 0x7F) {
            result.push_back(static_cast<char>(symbol));
        } else
        if (symbol <= 0x7FF) {
            result.push_back(static_cast<char>(0xC0 | ((symbol >> 6)  & 0x1F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else
        if (symbol <= 0xFFFF) {
            result.push_back(static_cast<char>(0xE0 | ((symbol >> 12) & 0x0F)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 6)  & 0x3F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else
        if (symbol <= 0x10FFFF) {
            result.push_back(static_cast<char>(0xF0 | ((symbol >> 18) & 0x07)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 12) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | ((symbol >> 6)  & 0x3F)));
            result.push_back(static_cast<char>(0x80 |  (symbol        & 0x3F)));
        } else
            throw std::invalid_argument("Invalid UTF-32 code symbol: " +
                TConvert<string, uint32_t>::convert(symbol, 16)); // ----->
    }
    return result; // ----->
}


u16string TConvert<u16string, string>::convert(string const &value) {
    // todo: test, not complete errors handling
    std::u16string result;
    auto length = value.length();
    size_t i = 0;
    while (i < length) {
        uint32_t symbol = static_cast<uint8_t>(value[i]);

        if (symbol <= 0x7F) {
            result.push_back(static_cast<char16_t>(symbol));
        } else
        if (symbol >= 0xC0 && symbol <= 0xDF && i + 1 < length) {
            symbol = ((symbol & 0x1F) << 6) |
                 (static_cast<uint8_t>(value[i + 1]) & 0x3F);
            i++;
            result.push_back(static_cast<char16_t>(symbol));
        } else
        if (symbol >= 0xE0 && symbol <= 0xEF && i + 2 < length) {
            symbol = ((symbol & 0x0F) << 12) |
                ((static_cast<uint8_t>(value[i + 1]) & 0x3F) << 6) |
                 (static_cast<uint8_t>(value[i + 2]) & 0x3F);
            i += 2;
            result.push_back(static_cast<char16_t>(symbol));
        } else
        if (symbol >= 0xF0 && symbol <= 0xF7 && i + 3 < length) {
            symbol = ((symbol & 0x07) << 18) |
                ((static_cast<uint8_t>(value[i + 1]) & 0x3F) << 12) |
                ((static_cast<uint8_t>(value[i + 2]) & 0x3F) << 6)  |
                 (static_cast<uint8_t>(value[i + 3]) & 0x3F);
            i += 3;
            symbol -= 0x10000;
            result.push_back(static_cast<char16_t>((symbol >> 10)   + 0xD800));
            result.push_back(static_cast<char16_t>((symbol & 0x3FF) + 0xDC00));
        } else
            throw std::invalid_argument("convertion utf8 to utf16 error: invalid code point on position " +
                TConvert<string, uint64_t>::convert(i)); // ----->
        i++;
    }
    return result; // ----->
}


u32string TConvert<u32string, string>::convert(string const &value) {
    // todo: test, not complete errors handling
    std::u32string result;
    size_t length = value.length();
    size_t i = 0;
    while (i < length) {
        uint32_t symbol = static_cast<uint8_t>(value[i]);

        if (symbol <= 0x7F) {
            result.push_back(symbol);
        } else
        if (symbol >= 0xC0 && symbol <= 0xDF && i + 1 < length) {
            symbol = ((symbol & 0x1F) << 6) |
                 (static_cast<uint8_t>(value[i + 1]) & 0x3F);
            i += 1;
            result.push_back(symbol);
        } else
        if (symbol >= 0xE0 && symbol <= 0xEF && i + 2 < length) {
            symbol = ((symbol & 0x0F) << 12) |
                ((static_cast<uint8_t>(value[i + 1]) & 0x3F) << 6) |
                 (static_cast<uint8_t>(value[i + 2]) & 0x3F);
            i += 2;
            result.push_back(symbol);
        } else
        if (symbol >= 0xF0 && symbol <= 0xF7 && i + 3 < length) {
            symbol = ((symbol & 0x07) << 18) |
                ((static_cast<uint8_t>(value[i + 1]) & 0x3F) << 12) |
                ((static_cast<uint8_t>(value[i + 2]) & 0x3F) << 6) |
                 (static_cast<uint8_t>(value[i + 3]) & 0x3F);
            i += 3;
            result.push_back(symbol);
        } else
            throw std::invalid_argument("convertion utf8 to utf32 error: invalid code point on position " +
                TConvert<string, uint64_t>::convert(i)); // ----->
        i++;
    }
    return result; // ----->
}


#endif // STL > C++14


string TConvert<string, string>::convert(string const &value) {
    return value; // ----->
}


} // iridium::convertion::implementation


//template<>
//high_resolution_clock::time_point TConvert::convert(string const &value) {
//    if (value.size() != time_scan_format_size)
//        throw runtime_error("convert '" + value + "' to time_t error, wrong source string format"); // ----->
//
//    struct std::tm  tm_ = {};
//    int ms      = 0;
//    int result  = platform::sscanf(value.c_str(), time_scan_format.c_str(),
//        &tm_.tm_year,
//        &tm_.tm_mon,
//        &tm_.tm_mday,
//        &tm_.tm_hour,
//        &tm_.tm_min,
//        &tm_.tm_sec,
//        &ms);
//
//    tm_.tm_year -= 1900;
//    tm_.tm_mon  -= 1;
//
//    if (result == 7) {
//        auto time = platform::mkgmtime(&tm_);
//
//        if (time < 0)
//            throw runtime_error("convert '" + value + "' to time_t error, mkgmtime error"); // ----->
//
//        return high_resolution_clock::time_point(std::chrono::seconds(time)) + std::chrono::milliseconds(ms); // ----->
//    } else
//        throw runtime_error("convert '" + value + "' to time_t error, sscanf: wrong source string format"); // ----->
//}


//// default format: "%Y-%m-%d %H:%M:%S%z %Z"
//template<>
//std::chrono::system_clock::time_point TConvert::convert(std::string const &source, std::string const &format) {
//    std::tm tm_ = {};
//    std::istringstream ss(source);
//    ss >> std::get_time(&tm_, format.c_str());
//    return std::chrono::system_clock::from_time_t(mktime(&tm_));
//}
//std::string dateTimeToString(date_time time) {
//    std::time_t now_c = std::chrono::system_clock::to_time_t(time);
//    auto tm = std::localtime(&now_c);
//    char buffer[32];
//    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S%z %Z", tm);
//    return std::string(buffer);
//}
