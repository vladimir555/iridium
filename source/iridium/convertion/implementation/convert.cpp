#include "convert.h"



#include <chrono>
#include <algorithm>
#include <codecvt>
#include <stdexcept>
#include <locale>
#include <sstream>
#include <cmath>

#include "iridium/strings.h"


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
string const time_format_unix               = "%Y-%m-%d %H:%M:%S";
string const time_scan_format               = "%04d-%02d-%02d %02d:%02d:%02d.%03d";
size_t const time_scan_format_size          = 23;
size_t const double_to_string_buffer_size   = 512;
size_t const int_to_string_buffer_size      = 64;
size_t const time_to_string_buffer_size     = 64;


} // unnamed


#include "iridium/platform.h"
#include PLATFORM_HEADER(convert.h)


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
    auto value_ms   = duration_cast<milliseconds>(value.time_since_epoch()).count();
    auto ms         = value_ms % 1000;
    time_t t        = system_clock::to_time_t(value);
    auto tm_        = platform::gmtime(&t);

    if (tm_) {
        char buffer[time_to_string_buffer_size];
        strftime(buffer, time_to_string_buffer_size, time_format_unix.c_str(), tm_);
        return string(buffer) + "." + rjust(convert<string>(ms), 3, '0'); // ----->
    } else
        throw runtime_error("convert time_t (gmtime) to string error"); // ----->
}


template<>
string convert(hours const &value) {
    return convert<string>(value.count()) + " h";
}


template<>
string convert(minutes const &value) {
    return convert<string>(value.count()) + " m";
}


template<>
string convert(seconds const &value) {
    return convert<string>(value.count()) + " s";
}


template<>
string convert(milliseconds const &value) {
    return convert<string>(value.count()) + " ms";
}


template<>
string convert(microseconds const &value) {
    return convert<string>(value.count()) + " mcs";
}


template<>
string convert(nanoseconds const &value) {
    return convert<string>(value.count()) + " ns";
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
    platform::sprintf(buffer, p.c_str(), value);
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
    int result = platform::sscanf(value.c_str(), time_scan_format.c_str(),
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

    double d = atof(value.c_str());

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
