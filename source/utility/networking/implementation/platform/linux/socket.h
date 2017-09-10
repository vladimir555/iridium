#ifndef HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4
#define HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4


#include "utility/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/socket.h"


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


using unix::CSocket;


} // platform
} // implementation
} // networking
} // utility


#endif // LINUX_PLATFORM



#endif // HEADER_SOCKET_07851BE9_9CF3_4DA7_9978_85E7575152E4
