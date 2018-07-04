#include "socket_server.h"



#include "utility/threading/implementation/thread.h"
#include "utility/threading/implementation/worker_pool.h"
#include "utility/threading/implementation/recursive_mutex.h"
#include "utility/threading/synchronized_scope.h"
#include "utility/io/implementation/listener.h"
#include "utility/io/protocol/http/implementation/protocol.h"


using utility::threading::implementation::CRecursiveMutex;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorkerPool;
using utility::threading::IWorkerPool;
using utility::threading::IJob;
using utility::io::implementation::CListener;
using utility::io::IListener;
using utility::io::IStream;
using utility::io::protocol::IProtocol;
using utility::io::protocol::http::implementation::CProtocol;
using utility::convertion::convert;

using std::map;
using std::string;


namespace utility {
namespace io {
namespace net {
namespace implementation {


CSocketServer::CSocketServer(
    URL const &url,
    int const &threads_count,
    protocol::IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_thread_acceptor   (CThread::create(CAcceptor::create(url, threads_count, protocol_factory), "socket_acceptor"))
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
    m_listener      (CListener::create()),
    m_socket        (CSocket::create(url, true)),
    m_stream_storage(StreamStorage::create())
{
    if (threads_count > 0) {
        IWorkerPool<IListener::Event::TSharedPtr>::TWorkerHandlers handlers;
        for (int i = 0; i < threads_count; i++) {
            auto handler = CSocketHandler::create(protocol_factory->create(), m_stream_storage);
            handlers.push_back(handler);
        }

        m_worker_pool = CWorkerPool<IListener::Event::TSharedPtr>::create("socket_handler", handlers);
    } else
        throw std::runtime_error("socket acceptor threads count must be greater than 0"); // ----->
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


CSocketServer::CAcceptor::StreamStorage::StreamStorage()
:
    Synchronized(CRecursiveMutex::create())
{}


StreamContext::TSharedPtr CSocketServer::CAcceptor::StreamStorage::get(int const &id) {
    LOCK_SCOPE
    return m_map_id_stream_context[id];
}


void CSocketServer::CAcceptor::StreamStorage::set(int const &id, StreamContext::TSharedPtr &stream) {
    LOCK_SCOPE
    m_map_id_stream_context[id] = stream;
}


CSocketServer::CAcceptor::CSocketHandler::CSocketHandler(protocol::IProtocol::TSharedPtr const &protocol, StreamStorage::TSharedPtr const &stream_storage)
:
    m_protocol      (protocol),
    m_stream_storage(stream_storage)
{}


void CSocketServer::CAcceptor::CSocketHandler::initialize() {}


void CSocketServer::CAcceptor::CSocketHandler::finalize() {}


CSocketServer::CAcceptor::CSocketHandler::TItems CSocketServer::CAcceptor::CSocketHandler::handle(TItems const &items) {
    for (auto const &event: items) {
        auto stream_context = m_stream_storage->get(event->stream->getID());

        if (!stream_context) {
            stream_context  = StreamContext::create(event->stream, m_protocol);
            m_stream_storage->set(event->stream->getID(), stream_context);
        }

        stream_context->update(event->event);
    }
    return TItems();
}


} // implementation
} // net
} // io
} // utility
