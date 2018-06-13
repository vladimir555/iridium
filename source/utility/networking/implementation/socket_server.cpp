#include "socket_server.h"



#include "utility/threading/implementation/thread.h"
#include "utility/threading/implementation/worker_pool.h"
#include "utility/threading/implementation/recursive_mutex.h"
#include "utility/threading/synchronized_scope.h"
#include "utility/io/implementation/listener.h"
#include "utility/protocol/http/implementation/protocol.h"


using utility::threading::implementation::CRecursiveMutex;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorkerPool;
using utility::threading::IWorkerPool;
using utility::threading::IJob;
using utility::io::implementation::CListener;
using utility::io::IListener;
using utility::io::IStream;
using utility::protocol::IProtocol;
using utility::protocol::http::implementation::CProtocol;
using utility::convertion::convert;

using std::map;
using std::string;


namespace utility {
namespace networking {
namespace implementation {


CSocketServer::CSocketServer(
    URL const &url,
    int const &threads_count,
    protocol::IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_thread_acceptor   (CThread::create(CAcceptor::create(url, threads_count, protocol_factory), "socket_acceptor")),
    m_protocol_factory  (protocol_factory)
{}


void CSocketServer::initialize() {
    m_thread_acceptor->initialize();
}


void CSocketServer::finalize() {
    m_thread_acceptor->finalize();
}


CSocketServer::CAcceptor::CAcceptor(
    URL const &url,
    int const &threads_count,
    protocol::IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_socket    (CSocket::create(url, true)),
    m_listener  (CListener::create())
{
    if (threads_count <= 0)
        throw std::runtime_error("socket acceptor threads count is 0"); // ----->

    IWorkerPool<IListener::Event::TSharedPtr>::TWorkerHandlers handlers;
    for (int i = 0; i < threads_count; i++) {
        auto handler = CSocketHandler::create(protocol_factory->create());
        handlers.push_back(handler);
    }

    m_worker_pool = CWorkerPool<IListener::Event::TSharedPtr>::create("socket_handler", handlers);
}


void CSocketServer::CAcceptor::initialize() {
    m_listener->initialize();
    m_socket->initialize();
    m_worker_pool->initialize();
}


void CSocketServer::CAcceptor::finalize() {
    m_worker_pool->finalize();
    m_socket->finalize();
    m_listener->finalize();
}


void CSocketServer::CAcceptor::run(std::atomic<bool> &is_running) {
    m_listener->add(m_socket);
    while (is_running)
        m_worker_pool->push(m_listener->wait());
}


CSocketServer::StreamLocker::StreamLocker()
:
    Synchronized(CRecursiveMutex::create())
{}


io::IStream::TSharedPtr CSocketServer::StreamLocker::get(int const &id) {
    LOCK_SCOPE
    return m_map_id_stream[id];
}


void CSocketServer::StreamLocker::set(int const &id, io::IStream::TSharedPtr &stream) {
    LOCK_SCOPE
    m_map_id_stream[id] = stream;
}


CSocketServer::CSocketHandler::CSocketHandler(protocol::IProtocol::TSharedPtr const &protocol)
:
    m_protocol(protocol)
{}


void CSocketServer::CSocketHandler::initialize() {}


void CSocketServer::CSocketHandler::finalize() {}


CSocketServer::CSocketHandler::TItems CSocketServer::CSocketHandler::handle(TItems const &items) {
    for (auto const &event: items) {
//        event->event;
    }
}


CSocketServer::CSocketHandler::StreamHandler::StreamHandler(
    protocol::IProtocol::TSharedPtr const &protocol,
    io::IStream::TSharedPtr         const &stream
) {

}


bool CSocketServer::CSocketHandler::StreamHandler::handle(IListener::Event const &event) {

}


} // implementation
} // networking
} // utility
