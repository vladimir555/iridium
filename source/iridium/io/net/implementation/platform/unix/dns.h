// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33
#define HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"


namespace iridium::io::net::implementation::platform {


/// \~english @brief Resolves a hostname to an IPv4 address using Unix platform services (e.g., `getaddrinfo`).
/// \~russian @brief Разрешает имя хоста в IPv4-адрес, используя службы платформы Unix (например, `getaddrinfo`).
/// \~english @param host The hostname to resolve.
/// \~russian @param host Имя хоста для разрешения.
/// \~english @return A `URI::TIPv4` structure representing the resolved IPv4 address.
/// \~russian @return Структура `URI::TIPv4`, представляющая разрешенный IPv4-адрес.
/// \~english @throws std::runtime_error if the host cannot be resolved or if an error occurs during resolution.
/// \~russian @throws std::runtime_error, если имя хоста не может быть разрешено или если во время разрешения возникает ошибка.
URI::TIPv4 getIPv4ByHost(std::string const &host);


} // iridium::io::net::implementation::platform


#endif // HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#endif // UNIX_PLATFORM
