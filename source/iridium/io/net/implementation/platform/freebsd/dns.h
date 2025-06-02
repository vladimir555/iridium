// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_395D3716_78D8_4AE4_8846_1C2528412A2E
#define HEADER_DNS_395D3716_78D8_4AE4_8846_1C2528412A2E


#include "iridium/platform.h"


/// \~english @file dns.h
/// \~english @brief Provides platform-specific DNS resolution for FreeBSD.
/// \~english This implementation includes and uses the Unix platform's DNS resolution functionality,
/// \~english leveraging the common POSIX-based services available on FreeBSD.
/// \~russian @file dns.h
/// \~russian @brief Предоставляет платформо-специфичное разрешение DNS для FreeBSD.
/// \~russian Данная реализация включает и использует функциональность разрешения DNS платформы Unix,
/// \~russian используя общие POSIX-совместимые службы, доступные на FreeBSD.

#ifdef FREEBSD_PLATFORM


#include "../unix/dns.h"


#endif // FREEBSD_PLATFORM


#endif // HEADER_DNS_395D3716_78D8_4AE4_8846_1C2528412A2E
