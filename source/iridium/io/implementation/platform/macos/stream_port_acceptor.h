#ifndef HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596
#define HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/stream_port_acceptor.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortAcceptor CStreamPortAcceptor;


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_62CEB972_E362_4EB2_8591_9B397CAFB596
