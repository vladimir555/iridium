#include "iridium/strings.h"

#include "convert.h"

#include <chrono>
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
using std::tm;
using std::runtime_error;
using std::wstring_convert;
using std::codecvt_utf8_utf16;
using std::codecvt_utf8;
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
string convert(system_clock::time_point const &value) {
    auto  value_ms  = duration_cast<milliseconds>(value.time_since_epoch()).count();
    uint32_t    ms  = value_ms % 1000;
    time_t      t   = system_clock::to_time_t(value);
    struct tm   tm_ = {};

    platform::gmtime_r(&t, &tm_);

    char buffer[time_to_string_buffer_size];
    strftime(buffer, time_to_string_buffer_size, time_format_unix, &tm_);
    return string(buffer) + "." + rjust(convert<string>(ms), 3, '0'); // ----->
}


template<>
string convert(hours const &value) {
    return convert<string, hours::rep>(static_cast<uint32_t>(value.count())) + " hours";
}


template<>
string convert(minutes const &value) {
    if ((value.count() % 60) == 0)
        return convert<string>(hours(value.count() % 60));
    return convert<string, minutes::rep>(value.count()) + " minutes";
}


template<>
string convert(seconds const &value) {
    if ((value.count() % 60) == 0)
        return convert<string>(minutes(value.count() % 60));
    return convert<string, seconds::rep>(value.count()) + " seconds";
}


template<>
string convert(milliseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(seconds(value.count() % 1000));
    return convert<string, milliseconds::rep>(value.count()) + " milliseconds";
}


template<>
string convert(microseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(milliseconds(value.count() % 1000));
    return convert<string, microseconds::rep>(value.count()) + " microseconds";
}


template<>
string convert(nanoseconds const &value) {
    if ((value.count() % 1000) == 0)
        return convert<string>(microseconds(value.count() % 1000));
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
        throw runtime_error("convert '" + value + "' to uint8 error"); // ----->
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


template<>
string convert(wstring const &value) {
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(value); // ----->
}


template<>
wstring convert(string const &value) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(value); // ----->
}


} // implementation
} // convertion
} // iridium
