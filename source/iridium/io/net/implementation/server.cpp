#include "server.h"
#include "socket_acceptor.h"
#include "iridium/io/service.h"
#include "iridium/io/implementation/stream_pool.h"
#include "iridium/threading/implementation/thread.h"


using iridium::threading::implementation::CThread;
using iridium::io::implementation::CStreamPool;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


CServer::CServer(URL const &url, protocol::IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_thread(CThread::create(CRunnable::create(url, protocol_factory), "acceptor"))
{}


void CServer::initialize() {
    m_thread->initialize();
}


void CServer::finalize() {
    m_thread->finalize();
}


CServer::CRunnable::CRunnable(URL const &url, protocol::IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_socket            (CSocketAcceptor::create(url)),
    m_stream_pool       (CStreamPool::create()),
    m_protocol_factory  (protocol_factory)
{}


void CServer::CRunnable::initialize() {
    m_socket->initialize();
    m_stream_pool->initialize();

    Service::instance().add(m_socket);
}


void CServer::CRunnable::finalize() {
    Service::instance().del(m_socket);

    m_stream_pool->finalize();
    m_socket->finalize();
}


void CServer::CRunnable::run(std::atomic<bool> &is_running) {
    while (is_running) {
        for (auto const &event: Service::instance().wait()) {
            if (event->stream->getID() == m_socket->getID()) {
                while (auto peer = m_socket->accept()) {
                    m_stream_pool->add(peer, m_protocol_factory->createProtocolHandler());
                }
            }
        }
    }
}


} // implementation
} // net
} // io
} // iridium
