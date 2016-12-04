#include "convert.h"


#include <chrono>
#include <algorithm>
#include <codecvt>
#include <stdexcept>
#include <locale>
#include <sstream>


using std::string;
using std::runtime_error;
using std::chrono::system_clock;
using std::thread;
using std::stringstream;
using std::wstring_convert;
using std::codecvt_utf8;
using std::codecvt_utf8_utf16;


namespace {


auto const time_format                  = "%Y-%m-%d %H:%M:%S";
auto const time_scan_format             = "%04i-%02i-%02i %02i:%02i:%02i";
auto const time_scan_format_size        = 23;
auto const double_to_string_buffer_size = 32;
auto const int_to_string_buffer_size    = 32;
auto const time_to_string_buffer_size   = 64;


} // unnamed


#include "utility/platform.h"
#include PLATFORM_HEADER(convert.h)


namespace utility {
namespace convertion {
namespace implementation {


template<>
string convert(bool const &value) {
    if (value)
        return "true"; // ----->
    else
        return "false"; //----->
}


template<>
string convert(int32_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(int32_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(uint32_t const &value, int const &base) {
    return convert<string>(static_cast<int32_t>(value), base); // ----->
}


template<>
string convert(uint32_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(int64_t const &value, int const &base) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(value, buffer, base);
    return buffer; // ----->
}


template<>
string convert(int64_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(uint64_t const &value, int const &base) {
    return convert<string>(static_cast<int64_t>(value), base); // ----->
}


template<>
string convert(uint64_t const &value) {
    return convert<string>(value, 10); // ----->
}


template<>
string convert(double const &value, int const &precission) {
    string p = string("%0.") + convert<string>(precission) + "lf";
    char buffer[double_to_string_buffer_size];
    platform::sprintf(buffer, p.c_str(), value);
    return buffer; // ----->
}


template<>
string convert(double const &value) {
    return convert<string>(value, 5);
}


template<>
string convert(system_clock::time_point const &value) {
    time_t t = system_clock::to_time_t(value);
    auto tm_ = platform::gmtime(&t);

    if (tm_) {
        char buffer[time_to_string_buffer_size];
        strftime(buffer, time_to_string_buffer_size, time_format, tm_);
        return string(buffer) + ".000"; // ----->
    } else
        throw runtime_error("convertion time to string error"); // ----->
}


template<>
string convert(std::string const &value) {
    return value; // ----->
}


template<>
string convert(std::wstring const &value) {
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(value); // ----->
}


template<>
string convert(thread::id const &value) {
    stringstream ss;
    ss << value;
    return ss.str(); // ----->
}


template<>
bool convert(std::string const &value_) {
    string value = value_;

    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (value == "true")
        return true; // ----->
    if (value == "false")
        return false; // ----->

    throw runtime_error("convertion '" + value_ + "' to bool error"); // ----->
}


//todo: base
template<>
int32_t convert(std::string const &value) {
    for (auto const ch: value)
        if ((ch < '0' || ch > '9') && ch != '-')
            throw runtime_error("convertion '" + value + "' to int32 error"); // ----->

    int32_t i = atoi(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convertion '" + value + "' to int32 error"); // ----->
    } else
        return i; // ----->
}


template<>
uint32_t convert(std::string const &value) {
    for (auto const ch: value)
        if (ch < '0' || ch > '9')
            throw runtime_error("convertion '" + value + "' to int32 error"); // ----->

    int32_t i = atoi(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convertion '" + value + "' to int32 error"); // ----->
    } else
        return i; // ----->
}


template<>
int64_t convert(std::string const &value) {
    for (auto const ch: value)
        if ((ch < '0' || ch > '9') && ch != '-')
            throw runtime_error("convertion '" + value + "' to int64 error");

    int64_t i = atol(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convertion '" + value + "' to int64 error"); // ----->
    } else
        return i; // ----->
}


template<>
uint64_t convert(std::string const &value) {
    for (auto const ch: value)
        if (ch < '0' || ch > '9')
            throw runtime_error("convertion '" + value + "' to int64 error");

    int64_t i = atol(value.c_str());

    if (i == 0) {
        auto result = platform::sscanf(value.c_str(), "%i", &i);
        if (result == 1)
            return i; // ----->
        else
            throw runtime_error("convertion '" + value + "' to int64 error"); // ----->
    } else
        return i; // ----->
}


template<>
double convert(std::string const &value) {
    for (auto const &ch: value)
        if ((ch < '0' || ch > '9') && ch != '.' && ch !='-')
            throw runtime_error("convertion '" + value + "' to double error"); // ----->

    double d = atof(value.c_str());

    if (d == 0.0) {
        auto result = platform::sscanf(value.c_str(), "%lf", &d);
        if (result == 1)
            return d; // ----->
        else
            throw runtime_error("convertion '" + value + "' to double error"); // ----->
    } else
        return d; // ----->
}


template<>
system_clock::time_point convert(string const &value) {
    if (value.size() != time_scan_format_size)
       throw runtime_error("convertion 1'" + value + "' to time error");

    struct std::tm tm_ = {};
    auto result = platform::sscanf(value.c_str(), time_scan_format,
        &tm_.tm_year,
        &tm_.tm_mon,
        &tm_.tm_mday,
        &tm_.tm_hour,
        &tm_.tm_min,
        &tm_.tm_sec);

    tm_.tm_year -= 1900;
    tm_.tm_mon  -= 1;

    if (result == 6) {
        auto time = platform::mkgmtime(&tm_);

        if (time < 0)
            throw runtime_error("convertion 2'" + value + "' to time error"); // ----->

        return system_clock::from_time_t(time); // ----->
    } else
        throw runtime_error("convertion 3'" + value + "' to time error"); // ----->
}


template<>
std::wstring convert(std::string const &value) {
    wstring_convert<codecvt_utf8_utf16<wchar_t> > converter;
    return converter.from_bytes(value); // ----->
}


} // implementation
} // convertion
} // utility
