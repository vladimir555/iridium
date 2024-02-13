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



using std::chrono::high_resolution_clock;
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
size_t const int_to_string_buffer_size      = 64;
size_t const time_to_string_buffer_size     = 64;


} // unnamed


#include "iridium/platform.h"
#include PLATFORM_HEADER(convert.h)


#include <iostream>
namespace iridium {
namespace convertion {
namespace implementation {


std::atomic<int> config::double_precission(5);


//template<>
//string convert(high_resolution_clock::time_point const &value) {
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
template<>
string convert(system_clock::time_point const &value, bool const &is_gmt_time) {
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
    return string(buffer) + "." + rjust(convert<string>(ms), 3, '0'); // ----->
}


template<>
string convert(system_clock::time_point const &value) {
    return convert<string>(value, true); // ----->
}


template<>
string convert(hours const &value) {
    return convert<string, hours::rep>(static_cast<uint32_t>(value.count())) + " hours"; // ----->
}


template<>
string convert(minutes const &value) {
    if ((value.count() % 60) == 0)
        return convert<string>(std::chrono::duration_cast<hours>(value));
    return convert<string, minutes::rep>(value.count()) + " minutes";
}


template<>
string convert(seconds const &value) {
    if ((value.count() % 60) == 0)
        return convert<string>(std::chrono::duration_cast<minutes>(value));
    return convert<string, seconds::rep>(value.count()) + " seconds";
}


template<>
string convert(milliseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(std::chrono::duration_cast<seconds>(value));
    return convert<string, milliseconds::rep>(value.count()) + " milliseconds";
}


template<>
string convert(microseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(std::chrono::duration_cast<milliseconds>(value));
    return convert<string, microseconds::rep>(value.count()) + " microseconds";
}


template<>
string convert(nanoseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(std::chrono::duration_cast<microseconds>(value));
    return convert<string, nanoseconds::rep>(value.count()) + " nanoseconds";
}


template<>
string convert(bool const &value) {
    if (value)
        return "true"; // ----->
    else
        return "false"; // ----->
}


template<>
string convert(int64_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(uint64_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return upperCase(buffer); // ----->
}


template<>
string convert(int32_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return upperCase(buffer); // ----->
}


template<>
string convert(uint32_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(uint16_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(uint8_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(int64_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(uint64_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(int32_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(uint32_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
std::string convert(int16_t const &value) {
    return convert<string>(static_cast<int32_t>(value)); // ----->
}


template<>
std::string convert(uint16_t const &value) {
    return convert<string>(static_cast<uint32_t>(value)); // ----->
}


template<>
std::string convert(uint8_t const &value) {
    return convert<string>(static_cast<uint32_t>(value)); // ----->
}


template<>
string convert(double const &value, int const &precision) {
    if (std::isnan(value))
        return "NAN"; // ----->

    string p = string("%0.") + convert<string>(precision) + "lf";
    char buffer[double_to_string_buffer_size];
    platform::snprintf(buffer, double_to_string_buffer_size, p.c_str(), value);
    return buffer; // ----->
}



template<>
string convert(double const &value) {
    return convert<string>(value, static_cast<int>(config::double_precission)); // ----->
}


template<>
std::string convert(std::thread::id const &value) {
    stringstream ss;
    ss << value;
    return ss.str();
}


//template<>
//high_resolution_clock::time_point convert(string const &value) {
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


template<>
system_clock::time_point convert(string const &value) {
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


//// default format: "%Y-%m-%d %H:%M:%S%z %Z"
//template<>
//std::chrono::system_clock::time_point convert(std::string const &source, std::string const &format) {
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





template<>
bool convert(string const &value_) {
    string value = value_;

    transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (value == "true")
        return true;  // ----->
    if (value == "false")
        return false; // ----->

    throw runtime_error("convert '" + value_ + "' to bool error"); // ----->
}


template<>
int32_t convert(string const &value) {
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


template<>
int64_t convert(string const &value) {
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


template<>
uint32_t convert(string const &value) {
    // todo: int test
    return convert<int32_t>(value); // ----->
}


template<>
uint16_t convert(string const &value) {
    auto result = convert<uint32_t>(value);
    if (result > UINT16_MAX)
        throw runtime_error("convert '" + value + "' to uint16 error"); // ----->
    return static_cast<uint16_t>(result); // ----->
}


template<>
uint8_t convert(string const &value) {
    auto result = convert<uint32_t>(value);
    if (result > UINT8_MAX)
        throw runtime_error("convert '" + value + "' to uint8 error"); // ----->
    return static_cast<uint8_t>(result); // ----->
}


template<>
uint64_t convert(string const &value) {
    return convert<int64_t>(value); // ----->
}


template<>
double convert(string const &value) {
    if (value == "NAN")
        return std::numeric_limits<double>::quiet_NaN(); // ----->

    for (auto const &ch: value)
        if ((ch < '0' || ch > '9') && ch != '.' && ch != '-')
            throw runtime_error("convert '" + value + "' to double error"); // ----->

    auto d = ::atof(value.c_str());

    if (d == 0.0) {
        int result = platform::sscanf(value.c_str(), "%lf", &d);
        if (result == 1)
            return d; // ----->
        else
            throw runtime_error("convert '" + value + "' to double error"); // ----->
    } else
        return d; // ----->
}


template<>
string convert(string const &value) {
    return value; // ----->
}


template<>
int convert(int const &value) {
    return value; // ----->
}


#if __cplusplus >= 201103L && __cplusplus < 201703L


template<>
string convert(wstring const &value) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(value); // ----->
}


template<>
wstring convert(string const &value) {
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
    return convert<string>(u16string(value.begin(), value.end())); // ----->
}


template<>
wstring convertInternal<wstring, string, 2>(string const &value) {
    auto result = convert<u16string>(value);
    return wstring(result.begin(), result.end());
}


#ifndef WINDOWS_PLATFORM


template<>
string convertInternal<string, wstring, 4>(wstring const &value) {
    return convert<string>(u32string(value.begin(), value.end())); // ----->
}


template<>
wstring convertInternal<wstring, string, 4>(string const &value) {
    auto result = convert<u32string>(value);
    return wstring(result.begin(), result.end());
}


#endif // !WINDOWS_PLATFORM


template<>
string convert(wstring const &value) {
    return convertInternal<string, wstring, sizeof(wchar_t)>(value); // ----->
}


template<>
wstring convert(string const &value) {
    return convertInternal<wstring, string, sizeof(wchar_t)>(value); // ----->
}


template<>
std::string convert(std::u16string const &value) {
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
            throw std::runtime_error("convert utf16 to utf8 error: invalid code point"); // ----->
    }
    return result; // ----->
}


template<>
std::string convert(std::u32string const &value) {
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
                convert<string>(static_cast<uint32_t>(symbol), 16)); // ----->
    }
    return result; // ----->
}


template<>
std::u16string convert(std::string const &value) {
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
            throw std::invalid_argument("Invalid UTF-8 code point at position " +
                convert<string>(i)); // ----->

        i++;
    }
    return result; // ----->
}


template<>
std::u32string convert(std::string const &value) {
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
            throw std::invalid_argument("Invalid UTF-8 code point at position " +
                convert<string>(i)); // ----->

        i++;
    }
    return result; // ----->
}


#endif // STL > C++14


} // implementation
} // convertion
} // iridium
