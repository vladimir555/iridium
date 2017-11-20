#ifndef HEADER_SOCKET_A5A8C70A_D465_41D3_8183_F8801D94A03D
#define HEADER_SOCKET_A5A8C70A_D465_41D3_8183_F8801D94A03D


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/networking/socket.h"


namespace utility {
namespace networking {
namespace client {


class ISocket: public pattern::IInitializable, public ISocketStream {
public:
    DEFINE_SMART_PTR(ISocket)
    virtual ~ISocket() = default;
};


} // client
} // networking
} // utility


#endif // HEADER_SOCKET_A5A8C70A_D465_41D3_8183_F8801D94A03D
