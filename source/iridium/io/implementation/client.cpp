#include "client.h"
#include "iridium/io/implementation/session_manager.h"
#include "iridium/io/implementation/stream_port.h"


namespace iridium {
namespace io {
namespace implementation {


CClient::CClient(URL const &url, IProtocol::TSharedPtr const &protocol)
:
    m_stream            (),
    m_protocol          (protocol),
    m_session_manager   (CSessionManager::create())
{}


void CClient::initialize() {
    m_stream->initialize();
    m_session_manager->initialize();
    m_session_manager->manage(m_stream, m_protocol);
}


void CClient::finalize() {
    m_session_manager->finalize();
    m_stream->finalize();
}


} // implementation
} // io
} // iridium
