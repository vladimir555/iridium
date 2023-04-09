// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_PEER_59393E2C_61A3_4D7B_9C6B_49EDA1293C16
#define HEADER_SOCKET_PEER_59393E2C_61A3_4D7B_9C6B_49EDA1293C16


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


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
