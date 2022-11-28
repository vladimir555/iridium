#ifndef HEADER_SOCKET_PEER_184F2E08_B4A6_43C0_B89D_6668BCAEBD13
#define HEADER_SOCKET_PEER_184F2E08_B4A6_43C0_B89D_6668BCAEBD13


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


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


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_PEER_184F2E08_B4A6_43C0_B89D_6668BCAEBD13
