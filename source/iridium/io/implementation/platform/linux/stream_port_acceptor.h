#ifndef HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75
#define HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_acceptor.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CStreamPortAcceptor CStreamPortAcceptor;


} // platform
} // implementation
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_CD2D8A08_F56C_4390_9031_8C06FED4BC75
