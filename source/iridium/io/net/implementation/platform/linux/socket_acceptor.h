// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9
#define HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/socket_acceptor.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocketAcceptor CSocketAcceptor;


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_5E4DE02B_D181_4DD6_9D59_5EC1FB6B92C9
