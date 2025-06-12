// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9
#define HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


/// \~english @file socket_acceptor.h
/// \~english @brief Defines the socket acceptor implementation for the Linux platform.
/// \~english This implementation typedefs to the common Unix `CSocketAcceptor` implementation,
/// \~english leveraging POSIX socket APIs available on Linux.
/// \~russian @file socket_acceptor.h
/// \~russian @brief Определяет реализацию принимающего сокета для платформы Linux.
/// \~russian Данная реализация использует typedef для общей Unix-реализации `CSocketAcceptor`,
/// \~russian используя POSIX API сокетов, доступные на Linux.

#include "../unix/socket_acceptor.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocketAcceptor CSocketAcceptor;


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9
