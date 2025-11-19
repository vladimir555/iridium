// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580
#define HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/uri.h"


namespace iridium::io::net::implementation::platform {


/// \~english @brief Resolves a hostname to an IPv4 address using Windows platform services (e.g., `getaddrinfo` or older `gethostbyname`).
/// \~russian @brief Разрешает имя хоста в IPv4-адрес, используя службы платформы Windows (например, `getaddrinfo` или более старый `gethostbyname`).
/// \~english @param host The hostname to resolve.
/// \~russian @param host Имя хоста для разрешения.
/// \~english @return A `URI::TIPv4` structure representing the resolved IPv4 address.
/// \~russian @return Структура `URI::TIPv4`, представляющая разрешенный IPv4-адрес.
/// \~english @throws std::runtime_error if the host cannot be resolved or if an error occurs during resolution (e.g., WSA error).
/// \~russian @throws std::runtime_error, если имя хоста не может быть разрешено или если во время разрешения возникает ошибка (например, ошибка WSA).
URI::TIPv4 getIPv4ByHost(std::string const &host);


} // iridium::io::net::implementation::platform


#endif // HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#endif // WINDOWS_PLATFORM
