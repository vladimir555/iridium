// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements macOS-specific type conversion specializations.
/// \~russian @file
/// @brief Реализует платформо-специфичные специализации преобразования типов для macOS.

#include <string>
#include "convert.h"

#ifdef MACOS_PLATFORM

using std::string;

namespace iridium::convertion::implementation {

string TConvert<string, long>::convert(long const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<int64_t>(value), buffer, 10);
    return string(buffer);
}

string TConvert<string, unsigned long>::convert(unsigned long const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<uint64_t>(value), buffer, 10);
    return string(buffer);
}

} // iridium::convertion::implementation

#endif // MACOS_PLATFORM
