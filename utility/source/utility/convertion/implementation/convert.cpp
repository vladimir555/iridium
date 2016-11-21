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


namespace {


auto const time_scan_format_size = 0;


} // unnamed


namespace utility {
namespace convertion {
namespace implementation {

//268
template<>
system_clock::time_point convert(string const &value) {
    if (value.size() != time_scan_format_size)
       throw runtime_error("convert '" + value + "' to time error");

    struct std::tm tm_ = {};
    //int result =
}


} // implementation
} // convertion
} // utility
