// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF
#define HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF


/// \~english @file
/// @brief Includes the Unix platform-specific conversion header for FreeBSD.
/// \~russian @file
/// @brief Включает заголовочный файл преобразования для платформы Unix для FreeBSD.


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


#include "../unix/convert.h"

#include "iridium/convertion/implementation/common.h"


namespace iridium::convertion::implementation {


template<>
struct TConvert<std::string, long long> {
    static std::string convert(long long const &value);
};


} // iridium::convertion::implementation


#endif


#endif // HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF
