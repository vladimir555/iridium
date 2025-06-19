#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF
#define HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/stream_port_ipc_client.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CStreamPortIPCClient CStreamPortIPCClient;


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_D7938B4E_10F9_41ED_B249_C8402B16E0BF
