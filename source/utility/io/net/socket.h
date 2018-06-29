#ifndef HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
#define HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6


#include "utility/io/stream.h"
#include "utility/pattern/initializable.h"
#include "url.h"


namespace utility {
namespace io {
namespace net {


class ISocket:
    public pattern::IInitializable,
    public io::IStream
{
public:
    DEFINE_INTERFACE(ISocket)

    virtual IStream::TSharedPtr accept() = 0;
    virtual URL getURL() const = 0;
};


} // net
} // io
} // utility


#endif // HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
