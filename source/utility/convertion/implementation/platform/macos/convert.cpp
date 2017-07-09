#include <string>

#include "utility/convertion/convert.h"


using std::string;
using utility::convertion::convert;


template<>
std::string utility::convertion::implementation::convert(long const &value) {
    return convert<string>(static_cast<int64_t>(value)); // ----->
}


template<>
std::string utility::convertion::implementation::convert(unsigned long const &value) {
    return convert<string>(static_cast<uint64_t>(value)); // ----->
}
