// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580
#define HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/uri.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


URI::TIPv4 getIPv4ByHost(std::string const &host);


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // HEADER_DNS_A166216A_6B6F_4336_B6BE_6E3D56AC1580


#endif // WINDOWS_PLATFORM
