#ifndef HEADER_SOCKET_BF5379C7_1015_41A7_B7EC_10B65F19DFB9
#define HEADER_SOCKET_BF5379C7_1015_41A7_B7EC_10B65F19DFB9


#include "utility/pattern/initializable.h"
#include "utility/smart_ptr.h"
#include "utility/threading/worker_pool.h"
#include "utility/net/socket.h"


namespace utility {
namespace net {
namespace server {


class ISocket: public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(ISocket)
    virtual ~ISocket() = default;

    typedef threading::IWorkerPool<net::ISocket::TEvent::TSharedPtr> TEventsWorkerPool;
    typedef TEventsWorkerPool::TWorkerHandler  TEventsHandler;
    typedef TEventsWorkerPool::TWorkerHandlers TEventsHandlers;
};


} // server
} // net
} // utility


#endif // HEADER_SOCKET_BF5379C7_1015_41A7_B7EC_10B65F19DFB9
