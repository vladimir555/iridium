#ifndef HEADER_HTTP_SERVER_0A4A892F_1A6E_40CD_9ADF_3624646EC77E
#define HEADER_HTTP_SERVER_0A4A892F_1A6E_40CD_9ADF_3624646EC77E


#include "utility/io/protocol/protocol.h"


namespace utility {
namespace io {
namespace protocol {


class HTTPServer: IProtocol {
public:
    DEFINE_IMPLEMENTATION(HTTPServer)
    HTTPServer() = default;
};


} // protocol
} // io
} // utility


#endif // HEADER_HTTP_SERVER_0A4A892F_1A6E_40CD_9ADF_3624646EC77E
