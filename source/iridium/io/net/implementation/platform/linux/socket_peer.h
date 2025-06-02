// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_PEER_59393E2C_61A3_4D7B_9C6B_49EDA1293C16
#define HEADER_SOCKET_PEER_59393E2C_61A3_4D7B_9C6B_49EDA1293C16


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


/// \~english @file socket_peer.h
/// \~english @brief Defines the peer socket implementation for the Linux platform.
/// \~english A peer socket typically represents a connection accepted by a server.
/// \~english This implementation typedefs to the common Unix `CSocketPeer` implementation,
/// \~english leveraging POSIX socket APIs available on Linux.
/// \~russian @file socket_peer.h
/// \~russian @brief Определяет реализацию сокета равноправного узла для платформы Linux.
/// \~russian Сокет равноправного узла обычно представляет соединение, принятое сервером.
/// \~russian Данная реализация использует typedef для общей Unix-реализации `CSocketPeer`,
/// \~russian используя POSIX API сокетов, доступные на Linux.

#include "../unix/socket_peer.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocketPeer CSocketPeer;


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_PEER_59393E2C_61A3_4D7B_9C6B_49EDA1293C16
