// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_9BAAB2D7_848C_4611_AC00_E2279F04F853
#define HEADER_DNS_9BAAB2D7_848C_4611_AC00_E2279F04F853


#include "iridium/platform.h"


/// \~english @file dns.h
/// \~english @brief Provides platform-specific DNS resolution for macOS.
/// \~english This implementation includes and uses the Unix platform's DNS resolution functionality,
/// \~english as macOS is a Unix-based system and shares common POSIX APIs.
/// \~russian @file dns.h
/// \~russian @brief Предоставляет платформо-специфичное разрешение DNS для macOS.
/// \~russian Данная реализация включает и использует функциональность разрешения DNS платформы Unix,
/// \~russian поскольку macOS является Unix-подобной системой и разделяет общие POSIX API.

#ifdef MACOS_PLATFORM


#include "../unix/dns.h"


#endif // MACOS_PLATFORM


#endif // HEADER_DNS_9BAAB2D7_848C_4611_AC00_E2279F04F853
