// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33
#define HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"


namespace iridium::io::net::implementation::platform {


URI::TIPv4 getIPv4ByHost(std::string const &host);


} // iridium::io::net::implementation::platform


#endif // HEADER_DNS_3797E08A_AF11_43C7_BE08_3B45CFCCCB33


#endif // UNIX_PLATFORM
