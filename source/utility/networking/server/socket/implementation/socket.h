#ifndef HEADER_SOCKET_CD651BC4_EA37_4328_B05D_024B00945420
#define HEADER_SOCKET_CD651BC4_EA37_4328_B05D_024B00945420


#include "utility/networking/server/socket/socket.h"

//#include <websocketpp/server.hpp>
//#include <websocketpp/config/core.hpp>
//#include <websocketpp/config/minimal_server.hpp>


namespace utility {
namespace networking {
namespace server {
namespace socket {
namespace implementation {


class CWebSocketpp: public ISocket {
public:
    ///
    CWebSocketpp() = default;
    ///
    virtual ~CWebSocketpp() = default;
    ///
    virtual void initialize() override;
    ///
    virtual void finalize() override;

private:
    //websocketpp::server<websocketpp::config::minimal_server> m_server;
};


} // implementation
} // socket
} // server
} // networking
} // utility


#endif // HEADER_SOCKET_CD651BC4_EA37_4328_B05D_024B00945420
