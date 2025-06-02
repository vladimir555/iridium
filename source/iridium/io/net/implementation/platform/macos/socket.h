// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_EF0CBDFF_315D_4BFE_8D59_0A44EE80B87C
#define HEADER_SOCKET_EF0CBDFF_315D_4BFE_8D59_0A44EE80B87C


/// \~english @file socket.h
/// \~english @brief Defines the base socket implementation for the macOS platform.
/// \~english This implementation currently typedefs `CSocket` to the common Unix `CSocketBase` implementation,
/// \~english leveraging the POSIX socket APIs available on macOS.
/// \~english More specific socket types (client, server, peer) for macOS would typically derive from this.
/// \~russian @file socket.h
/// \~russian @brief Определяет базовую реализацию сокета для платформы macOS.
/// \~russian Данная реализация в настоящее время использует typedef `CSocket` для общей Unix-реализации `CSocketBase`,
/// \~russian используя POSIX API сокетов, доступные на macOS.
/// \~russian Более специфичные типы сокетов (клиентские, серверные, равноправные узлы) для macOS обычно наследуются отсюда.

#include "../unix/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix_::CSocketBase CSocket;


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_EF0CBDFF_315D_4BFE_8D59_0A44EE80B87C
