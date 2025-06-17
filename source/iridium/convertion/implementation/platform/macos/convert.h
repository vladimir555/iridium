// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
#define HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/convert.h"


template<>
std::string iridium::convertion::implementation::convert(long const &value);


template<>
std::string iridium::convertion::implementation::convert(unsigned long const &value);


#endif // MACOS_PLATFORM


#endif // HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
