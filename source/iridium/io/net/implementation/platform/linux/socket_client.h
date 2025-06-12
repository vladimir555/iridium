// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_CLIENT_E0FB4D52_1718_432B_8E31_D61DDD3BDC75
#define HEADER_SOCKET_CLIENT_E0FB4D52_1718_432B_8E31_D61DDD3BDC75


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


/// \~english @file socket_client.h
/// \~english @brief Defines the client socket implementation for the Linux platform.
/// \~english This implementation typedefs to the common Unix `CSocketClient` implementation,
/// \~english leveraging POSIX socket APIs available on Linux.
/// \~russian @file socket_client.h
/// \~russian @brief Определяет реализацию клиентского сокета для платформы Linux.
/// \~russian Данная реализация использует typedef для общей Unix-реализации `CSocketClient`,
/// \~russian используя POSIX API сокетов, доступные на Linux.

#include "../unix/socket_client.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocketClient CSocketClient;


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_CLIENT_E0FB4D52_1718_432B_8E31_D61DDD3BDC75
