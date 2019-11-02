// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
#define HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6


#include "iridium/io/stream.h"
#include "iridium/pattern/initializable.h"
#include "url.h"


namespace iridium {
namespace io {
namespace net {


class ISocket: public IStreamPort {
public:
    DEFINE_INTERFACE(ISocket)

    virtual IStream::TSharedPtr accept() = 0;
    virtual URL getURL() const = 0;
};


} // net
} // io
} // iridium


#endif // HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
