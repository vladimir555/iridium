#ifndef HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
#define HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20


#include "implementation/convert.h"


namespace utility {
namespace convertion {


using implementation::convert;
using implementation::convertPtr;
using implementation::config;


} // convertion
} // utility


#define DEFINE_CONVERT(TTo, TFrom) \
template<> \
TTo utility::convertion::implementation::convert(TFrom const &value); \


#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
template<> \
TTo utility::convertion::implementation::convert(TFrom const &from) { \
    return TFunc(from); \
}


#endif // HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
