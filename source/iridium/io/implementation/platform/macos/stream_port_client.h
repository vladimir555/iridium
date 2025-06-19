#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_2BF041F3_0223_489D_A7BA_1BA3B0D5036A
#define HEADER_SHARED_MEMORY_STREAM_PORT_2BF041F3_0223_489D_A7BA_1BA3B0D5036A


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/stream_port_client.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::StreamPortClient StreamPortClient;


} // platform
} // implementation
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_2BF041F3_0223_489D_A7BA_1BA3B0D5036A
