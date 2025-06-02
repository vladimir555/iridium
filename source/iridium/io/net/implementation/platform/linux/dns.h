// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_C2E9A23C_4342_4222_AD05_3EF8336A12D8
#define HEADER_DNS_C2E9A23C_4342_4222_AD05_3EF8336A12D8


#include "iridium/platform.h"


/// \~english @file dns.h
/// \~english @brief Provides platform-specific DNS resolution for Linux.
/// \~english This implementation includes and uses the Unix platform's DNS resolution functionality,
/// \~english leveraging the common POSIX-based services available on Linux.
/// \~russian @file dns.h
/// \~russian @brief Предоставляет платформо-специфичное разрешение DNS для Linux.
/// \~russian Данная реализация включает и использует функциональность разрешения DNS платформы Unix,
/// \~russian используя общие POSIX-совместимые службы, доступные на Linux.

#ifdef LINUX_PLATFORM


#include "../unix/dns.h"


#endif // LINUX_PLATFORM



#endif // HEADER_DNS_C2E9A23C_4342_4222_AD05_3EF8336A12D8
