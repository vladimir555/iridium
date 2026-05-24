#ifndef HEADER_STREAM_PORT_PEER_02CEEF02_EDD6_43FE_B758_8C93BD24467D
#define HEADER_STREAM_PORT_PEER_02CEEF02_EDD6_43FE_B758_8C93BD24467D


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/stream_port_peer.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortPeer CStreamPortPeer;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_02CEEF02_EDD6_43FE_B758_8C93BD24467D
