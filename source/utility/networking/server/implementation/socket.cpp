#include "socket.h"

#include "utility/networking/implementation/socket.h"

#include "utility/threading/implementation/thread.h"


using std::string;
using utility::threading::implementation::CThread;
using utility::convertion::convert;


namespace utility {
namespace networking {
namespace server {
namespace implementation {


CSocket::CSocket(URL const &url, TSocketStreamQueue::TSharedPtr const &socket_stream_queue)
:
    m_runnuble  (Acceptor::create(url, socket_stream_queue)),
    m_thread    (CThread::create(m_runnuble, "socket acceptor " + convert<string>(url)))
{}


void CSocket::initialize() {
    m_thread->initialize();
}


void CSocket::finalize() {
    m_thread->finalize();
}


CSocket::Acceptor::Acceptor(URL const &url, TSocketStreamQueue::TSharedPtr const &socket_stream_queue)
:
    m_socket(networking::implementation::CSocket::create(url)),
    m_socket_stream_queue(socket_stream_queue)
{}


void CSocket::Acceptor::run() {
    m_socket->open();
    m_socket->listen();
    try {
        while(m_is_running)
            m_socket_stream_queue->push(m_socket->accept());
    } catch (std::exception const &e) {
        if (m_is_running) {
            m_socket->close();
            throw e; // ----->
        }
    }
}


void CSocket::Acceptor::stop() {
    CRunnuble::stop();
    m_socket->close();
}


} // implementation
} // server
} // networking
} // utility
