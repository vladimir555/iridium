// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
#define HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6


/// \~english @file
/// @brief Declares macOS-specific type conversion specializations.
/// \~russian @file
/// @brief Объявляет платформо-специфичные специализации преобразования типов для macOS.


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/convert.h"

#include "iridium/convertion/implementation/common.h"


namespace iridium::convertion::implementation {


/// \~english @brief Specialization for converting a `long` to a `std::string`.
/// \~russian @brief Специализация для преобразования `long` в `std::string`.
template<>
struct TConvert<std::string, long> {
    static std::string convert(long const &value);
};


/// \~english @brief Specialization for converting an `unsigned long` to a `std::string`.
/// \~russian @brief Специализация для преобразования `unsigned long` в `std::string`.
template<>
struct TConvert<std::string, unsigned long> {
    static std::string convert(unsigned long const &value);
};


} // iridium::convertion::implementation


#endif // MACOS_PLATFORM


#endif // HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
