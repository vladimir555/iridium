#include "client.h"
#include "iridium/io/implementation/stream_pool.h"


namespace iridium {
namespace io {
namespace implementation {


CClient::CClient(protocol::ISession::TSharedPtr const &session)
:
    m_session       (session),
    m_stream_pool   (CStreamPool::create())
{}


void CClient::initialize() {
    m_stream_pool->initialize();
    m_stream_pool->add(m_session);
}


void CClient::finalize() {
    m_stream_pool->del(m_session);
    m_stream_pool->finalize();
}


} // implementation
} // io
} // iridium
