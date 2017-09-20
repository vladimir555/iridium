#include "socket.h"

#include "utility/networking/implementation/socket.h"

#include "utility/threading/implementation/thread.h"
#include "utility/threading/implementation/worker_pool.h"

#include "utility/logging/logger.h"


using std::string;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorkerPool;
using utility::convertion::convert;


namespace utility {
namespace networking {
namespace server {
namespace implementation {


CSocket::CSocket(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers)
:
    m_runnuble  (Acceptor::create(url, socket_stream_handlers)),
    m_thread    (CThread::create(m_runnuble, "socket acceptor " + convert<string>(url)))
{}


void CSocket::initialize() {
    m_thread->initialize();
}


void CSocket::finalize() {
    m_thread->finalize();
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
        while(m_is_running)
            m_worker_pool->push(m_socket->accept());
    } catch (std::exception const &e) {
        if (m_is_running) {
            m_socket->close();
            throw e; // ----->
        }
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
