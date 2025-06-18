// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
#define HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20


#include "implementation/convert.h"


namespace iridium {
namespace convertion {


using implementation::convert;
using implementation::convertPtr;
using implementation::config;


} // convertion
} // iridium


#define DEFINE_CONVERT(TTo, TFrom) \
template<> \
TTo iridium::convertion::implementation::convert(TFrom const &value);



#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
template<> \
TTo iridium::convertion::implementation::convert(TFrom const &from) { \
    return TFunc(from); \
}


#endif // HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
