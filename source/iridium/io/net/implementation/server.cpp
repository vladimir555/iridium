#include "server.h"

#include "iridium/io/implementation/stream_pool.h"
#include "iridium/io/net/implementation/socket_acceptor.h"


using iridium::io::implementation::CStreamPool;
using iridium::io::protocol::IPeerSessionFactory;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


CServer::CServer(URL const &url, IPeerSessionFactory::TSharedPtr const &session_factory)
:
    m_stream_pool       (CStreamPool::create()),
    m_acceptor_session  (CAcceptorSession::create(url, session_factory, m_stream_pool))
{}


void CServer::initialize() {
    m_stream_pool->initialize();
    m_stream_pool->add(m_acceptor_session);
}


void CServer::finalize() {
    m_stream_pool->del(m_acceptor_session);
    m_stream_pool->finalize();
}


IStreamReader::TConstSharedPtr CServer::CAcceptorSession::CFakePipe::getReader() const {
    return nullptr; // ----->
}


IStreamWriter::TConstSharedPtr CServer::CAcceptorSession::CFakePipe::getWriter() const {
    return nullptr; // ----->
}


void CServer::CAcceptorSession::CFakePipe::set(IStreamReader::TSharedPtr const &, IStreamWriter::TSharedPtr const &) {}


bool CServer::CAcceptorSession::CFakePipe::transmit(IListener::Event::TConstSharedPtr const &) {
    return true; // ----->
};


CServer::CAcceptorSession::CAcceptorSession(
    URL                             const &url,
    IPeerSessionFactory::TSharedPtr const &session_factory,
    IStreamPool::TSharedPtr         const &stream_pool)
:
    m_url               (url),
    m_session_factory   (session_factory),
    m_stream_pool       (stream_pool)
{}


IStream::TSharedPtr CServer::CAcceptorSession::getStream(IListener::TSharedPtr const &listener) {
    m_socket = CSocketAcceptor::create(m_url, listener);
    m_socket->initialize();
    return m_socket;
}


IPipe::TSharedPtr CServer::CAcceptorSession::getPipe(IListener::Event::TConstSharedPtr const &event) {
    if (event->stream == m_socket)
        while (auto peer = m_socket->accept())
            m_stream_pool->add(m_session_factory->createSession(peer));

    return m_pipe = m_pipe ? m_pipe : CFakePipe::create();
}


} // implementation
} // net
} // io
} // iridium
