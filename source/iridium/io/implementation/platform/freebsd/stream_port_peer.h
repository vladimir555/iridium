// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_PEER_A61EC0EC_2853_408B_ADA4_17E9354820C0
#define HEADER_STREAM_PORT_PEER_A61EC0EC_2853_408B_ADA4_17E9354820C0


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "../unix/stream_port_peer.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortPeer CStreamPortPeer;


} // iridium::io::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_A61EC0EC_2853_408B_ADA4_17E9354820C0
