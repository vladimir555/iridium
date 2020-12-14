#ifndef HEADER_CLIENT_B2EDDA95_4575_4064_92D8_DA29DEB53567
#define HEADER_CLIENT_B2EDDA95_4575_4064_92D8_DA29DEB53567


#include "iridium/io/protocol/protocol_factory.h"
#include "iridium/io/net/url.h"
#include "iridium/io/net/client.h"
#include "iridium/io/stream_pool.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CClient: public IClient {
public:
    DEFINE_IMPLEMENTATION(CClient)

    CClient(
        URL const &url,
        protocol::IProtocolHandler::TSharedPtr
            const &protocol_handler);

    void initialize()   override;
    void finalize()     override;

private:
    IStreamPool::TSharedPtr                 m_stream_pool;
    IStreamPort::TSharedPtr                 m_stream_port;
    protocol::IProtocolHandler::TSharedPtr  m_protocol_handler;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_CLIENT_B2EDDA95_4575_4064_92D8_DA29DEB53567
