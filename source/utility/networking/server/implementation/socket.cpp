#include "socket.h"

#include "utility/networking/implementation/socket.h"
#include "utility/networking/implementation/socket_handler.h"

#include "utility/threading/implementation/thread.h"
#include "utility/threading/implementation/worker_pool.h"
#include "utility/threading/synchronized_scope.h"

#include "utility/logging/logger.h"
#include "utility/assert.h"


using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorkerPool;
using utility::threading::implementation::CMutex;
using utility::networking::implementation::CSocketHandler;
using utility::convertion::convert;
using utility::protocol::IProtocol;
using utility::protocol::IPacket;

using std::string;
using std::map;
using std::chrono::high_resolution_clock;
using std::chrono::seconds;


auto const DEFAULT_READ_TIMEOUT = seconds(5);


namespace utility {
namespace networking {
namespace server {
namespace implementation {


CSocket::CSocket(URL const &url, IProtocol::TSharedPtr const &protocol, int const &count)
:
    m_acceptor_thread(CThread::create(CAcceptor::create(url, protocol, count), "socket acceptor " + convert<string>(url)))
{}


void CSocket::initialize() {
    m_acceptor_thread->initialize();
}


void CSocket::finalize() {
    m_acceptor_thread->finalize();
}


CSocket::CAcceptor::CAcceptor(URL const &url, IProtocol::TSharedPtr const &protocol, int const &count)
:
    m_socket    (networking::implementation::CSocket::create(url)),
    m_protocol  (SynchronizedProtocolHandler::create(protocol))
{
    TEventsHandlers handlers;

    for (int i = 0; i < count; i++)
        handlers.push_back(CEventsHandler::create(m_protocol));

    LOGT << "create worker pool size " << handlers.size();
    m_worker_pool = threading::implementation::CWorkerPool<networking::ISocket::TEvent::TSharedPtr>::create(handlers, "socket_acceptor");
}


void CSocket::CAcceptor::initialize() {
    m_socket->open();
    m_socket->listen();
    m_worker_pool->initialize();
    LOGT << "start acceptor";
}


void CSocket::CAcceptor::finalize() {
    m_worker_pool->finalize();
    m_socket->close();
    LOGT << "stop acceptor";
}


void CSocket::CAcceptor::run() {
    try {
        while (m_is_running) {
            m_worker_pool->push(m_socket->accept());
//            auto items = m_socket->accept();
//            m_worker_pool->push(items);
        }
    } catch (std::exception const &e) {
        LOGF << "server socket acceptor " << m_socket->getURL() << " error: " << e.what();
        if (m_is_running)
            m_socket->close();
    }
}


CSocket::CAcceptor::SynchronizedProtocolHandler::SynchronizedProtocolHandler(IProtocol::TSharedPtr const &protocol)
:
    m_protocol(protocol)
{}


IPacket::TSharedPtr CSocket::CAcceptor::SynchronizedProtocolHandler::exchange(IPacket::TSharedPtr const &packet) {
    LOCK_SCOPE
    return m_protocol->exchange(packet); // ----->
}


CSocket::CAcceptor::CEventsHandler::CEventsHandler(protocol::IProtocol::TSharedPtr const &protocol)
:
    m_protocol(protocol)
{
    LOGT << "create event handler";
}


CSocket::CAcceptor::CEventsHandler::TItems CSocket::CAcceptor::CEventsHandler::handle(TItems const &events) {
    LOGT << "event handler, items count " << events.size();
    TItems next_events;
    for (networking::ISocket::TEvent::TSharedPtr const &event: events) {
        if (m_map_url_socket[event->socket->getURL()]) {
            LOGT << "event handler, socket handler from map";
            m_map_url_socket[event->socket->getURL()]->handle(event->action);
        } else {
            LOGT << "event handler, add socket handler to map";
            m_map_url_socket[event->socket->getURL()] = CSocketHandler::create(event->socket, m_protocol);
            m_map_url_socket[event->socket->getURL()]->handle(event->action);
        }

        if (event->action == networking::ISocket::TEvent::TAction::CLOSE)
            m_map_url_socket[event->socket->getURL()].reset();
    }

    return next_events;
}


void CSocket::CAcceptor::CEventsHandler::initialize() {
    LOGT << "initialize event handler";
}


void CSocket::CAcceptor::CEventsHandler::finalize() {
    LOGT << "finalize event handler";
}


} // implementation
} // server
} // networking
} // utility
