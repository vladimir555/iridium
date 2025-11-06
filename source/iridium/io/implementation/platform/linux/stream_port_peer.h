#ifndef HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0
#define HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_peer.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CStreamPortPeer CStreamPortPeer;


} // platform
} // implementation
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0
