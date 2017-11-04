#include "socket.h"

#include "utility/networking/implementation/socket.h"

#include "utility/threading/implementation/thread.h"
#include "utility/threading/implementation/worker_pool.h"
#include "utility/threading/synchronized_scope.h"

#include "utility/logging/logger.h"
#include "utility/assert.h"


using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorkerPool;
using utility::threading::implementation::CMutex;
using utility::convertion::convert;
using std::string;


namespace utility {
namespace networking {
namespace server {
namespace implementation {


CSocket::CSocket(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers)
:
    m_runnuble      (Acceptor::create(url, socket_stream_handlers)),
    m_thread        (CThread::create(m_runnuble, "socket acceptor " + convert<string>(url)))
{}


void CSocket::initialize() {
    m_thread->initialize();
}


void CSocket::finalize() {
    m_thread->finalize();
}


CSocket::CCachedSocketStream::CCachedSocketStream(ISocketStream::TSharedPtr const &source_socket)
:
    m_source_socket (assertExists(source_socket, "source socket is NULL"))
{}


void CSocket::CCachedSocketStream::write(TPacket const &packet) {
    m_source_socket->write(packet);
}


ISocketStream::TPacket CSocket::CCachedSocketStream::read() {
    auto p = m_source_socket->read();
    m_cache.insert(m_cache.end(), p.begin(), p.end());
    return m_cache; // ----->
}


void CSocket::CCachedSocketStream::close() {
    m_source_socket->close();
}


URL CSocket::CCachedSocketStream::getURL() const {
    return m_source_socket->getURL(); // ------>
}


CSocket::Acceptor::Acceptor(URL const &url, CSocket::TSocketStreamsHandlers const &socket_stream_handlers)
:
    m_socket        (networking::implementation::CSocket::create(url)),
    m_worker_pool   (CWorkerPool<ISocketStream::TSharedPtr>::create(socket_stream_handlers, "socket_acceptor_handler"))
{}


void CSocket::Acceptor::run() {
    m_worker_pool->initialize();
    try {
        m_socket->open();
        m_socket->listen();
        while(m_is_running) {
            networking::ISocket::TSocketStreams sockets;
            for (auto const &socket: m_socket->accept())
                sockets.push_back(CCachedSocketStream::create(socket));
            m_worker_pool->push(sockets);
        }
    } catch (std::exception const &e) {
        LOGF << "server socket " << m_socket->getURL() << " error: " << e.what();
        if (m_is_running)
            m_socket->close();
    }
    m_worker_pool->finalize();
}


void CSocket::Acceptor::stop() {
    CRunnuble::stop();
    m_socket->close();
}


} // implementation
} // server
} // networking
} // utility
