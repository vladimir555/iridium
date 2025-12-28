// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
#define HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20


#include "implementation/convert.h"


namespace iridium::convertion {


using implementation::config;


template<typename TResult, typename TValue, bool is_throwable = false>
TResult convert(TValue const &value) {
    return implementation::TConvertPolicy<TResult, TValue, void, is_throwable>::convert(value);
}


template<typename TResult, typename TValue, typename TFormat, bool is_throwable = false>
TResult convert(TValue const &value, TFormat const &format) {
    return implementation::TConvertPolicy<TResult, TValue, void, is_throwable>::convert(value, format);
}


} // iridium::convertion


#define DEFINE_CONVERT(TTo, TFrom) \
namespace iridium::convertion::implementation { \
template<> \
struct TConvert<TTo, TFrom> { \
    static TTo convert(TFrom const &value); \
}; \
}


#define IMPLEMENT_CONVERT(TTo, TFrom, TFunc) \
namespace iridium::convertion::implementation { \
TTo TConvert<TTo, TFrom>::convert(TFrom const &value) { \
    return TFunc(value); \
} \
}


#endif // HEADER_CONVERT_A1637EFD_3229_474D_BFEB_E9EAD7FF0C20
