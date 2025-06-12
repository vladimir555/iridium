// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file dns.h
/// \~english @brief Provides access to DNS resolution functionality.
/// \~english This header includes platform-specific DNS implementations and exposes
/// \~english the `getIPv4ByHost` function for resolving hostnames to IPv4 addresses
/// \~english within the `iridium::io::net` namespace.
/// \~russian @file dns.h
/// \~russian @brief Предоставляет доступ к функциональности разрешения DNS.
/// \~russian Этот заголовочный файл включает платформо-специфичные реализации DNS и предоставляет
/// \~russian функцию `getIPv4ByHost` для разрешения имен хостов в IPv4-адреса
/// \~russian в пространстве имен `iridium::io::net`.

#ifndef HEADER_DNS_60B2E420_B240_48F0_9E6E_B51284FAF8B6
#define HEADER_DNS_60B2E420_B240_48F0_9E6E_B51284FAF8B6


#include "implementation/dns.h"


namespace iridium {
namespace io {
namespace net {


using implementation::platform::getIPv4ByHost;


} // net
} // io
} // iridium


#endif // HEADER_DNS_60B2E420_B240_48F0_9E6E_B51284FAF8B6
