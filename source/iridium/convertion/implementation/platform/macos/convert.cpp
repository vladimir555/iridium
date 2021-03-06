/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <string>

#include "iridium/convertion/convert.h"

#include "convert.h"


#ifdef MACOS_PLATFORM


using std::string;
using iridium::convertion::convert;


template<>
std::string iridium::convertion::implementation::convert(long const &value) {
    return convert<string>(static_cast<int64_t>(value)); // ----->
}


template<>
std::string iridium::convertion::implementation::convert(unsigned long const &value) {
    return convert<string>(static_cast<uint64_t>(value)); // ----->
}


#endif // PLATFORM_MACOS
