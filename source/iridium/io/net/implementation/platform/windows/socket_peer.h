// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#pragma once


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/net/socket.h"
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
