#ifndef HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0
#define HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_peer.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortPeer CStreamPortPeer;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_57A8C17F_23C2_46C3_97DB_22A96F78AEC0
