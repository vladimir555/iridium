#include "client.h"


#include "iridium/io/implementation/stream_pool.h"
#include "iridium/io/net/implementation/socket_client.h"


using iridium::io::implementation::CStreamPool;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


CClient::CClient(URL const &url, protocol::IProtocolHandler::TSharedPtr const &protocol_handler)
:
    m_stream_pool       (CStreamPool::create()),
    m_stream_port       (CSocketClient::create(url)),
    m_protocol_handler  (protocol_handler)
{}


void CClient::initialize() {
    m_stream_pool->initialize();
    m_stream_pool->add(m_stream_port, m_protocol_handler);
}


void CClient::finalize() {
    m_stream_pool->del(m_stream_port);
    m_stream_pool->finalize();
}


} // implementation
} // net
} // io
} // iridium
