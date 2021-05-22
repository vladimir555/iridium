#pragma once


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/net/socket.h"
#include "iridium/io/listener.h"
#include "iridium/encryption/openssl.h"

#include "socket.h"
#include "socket_acceptor.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


class CSocketPeer : public ISocketStream {
public:
    DEFINE_IMPLEMENTATION(CSocketPeer)
    CSocketPeer() = default;
};


}
}
}
}
}


#endif // WINDOWS_PLATFORM
