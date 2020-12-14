/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "socket_server_acceptor.h"


#include "socket.h"
#include "peers.h"
#include "peer_handler.h"

#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/listener.h"


using iridium::threading::implementation::CAsyncQueue;
using iridium::threading::implementation::CWorkerPool;
using iridium::io::implementation::CListener;
using iridium::io::protocol::IProtocolFactory;

using iridium::threading::CSynchronized;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


//class CAsyncListener: public IListener {
//public:
//    DEFINE_IMPLEMENTATION(CAsyncListener)
//    CAsyncListener(IListener::TSharedPtr const &listener)
//    :
//        m_listener(listener),
//        m_queue_streams_to_del(CAsyncQueue<IStream::TSharedPtr>::create()),
//        m_queue_streams_to_add(CAsyncQueue<IStream::TSharedPtr>::create())
//    {}

//    void initialize() override {
//        m_listener->initialize();
//    }

//    void finalize() override {
//        m_queue_streams_to_del->interrupt();
//        m_queue_streams_to_add->interrupt();
//        m_listener->finalize();
//    }

//    void add(IStream::TSharedPtr const &stream) override {
//        m_queue_streams_to_add->push(stream);
//    }

//    void del(IStream::TSharedPtr const &stream) override {
//        m_queue_streams_to_del->push(stream);
//    }

//    TEvents wait() override {
//        for (auto const &stream : m_queue_streams_to_del->pop(false))
//            m_listener->del(stream);
//        for (auto const &stream : m_queue_streams_to_add->pop(false))
//            m_listener->add(stream);

//        return m_listener->wait(); // ----->
//    }

//private:
//    IListener::TSharedPtr m_listener;
//    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_queue_streams_to_del;
//    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_queue_streams_to_add;
//};


//CSocketServerAcceptor::CSocketServerAcceptor(
//    URL     const &url,
//    IProtocolFactory::TSharedPtr const &protocol_factory,
//    size_t  const &threads_count)
//:
//    m_protocol_factory  (protocol_factory),
//    m_listener          (CAsyncListener::create(CListener::create())),
//    m_socket            (CSocket::create(url, CSocket::TMode::SERVER)),
//    m_peers             (Peers::create(m_protocol_factory, m_listener))
//{
//    CWorkerPool<Peer::TSharedPtr>::TWorkerHandlers handlers;
//    for (size_t i = 0; i < threads_count; i++)
//        handlers.push_back(CPeerHandler::create());
//    m_worker_pool_peer_handler = CWorkerPool<Peer::TSharedPtr>::create("peer_handler", handlers);
//}


//// todo: peer timeout; peer finalization
//// todo: fix crashing on multithreading
//void CSocketServerAcceptor::run(std::atomic<bool> &is_running) {
//    while (is_running) {
//        for (auto const &event: m_listener->wait()) {
//            if (event->stream->getID() == m_socket->getID()) {
//                while (auto peer_socket = m_socket->accept()) {
//                    LOGT << "accepted peer socket fd is " << peer_socket->getID();
//                    auto peer   = m_peers->get(peer_socket);
//                    peer->event = Event::create(Event::TType::OPEN, peer_socket);
//                    peer->transmitter->setReader(peer_socket);
////                    LOGT << "push by acceptor new peer, fd " << peer_socket->getID();
//                    m_worker_pool_peer_handler->push(peer);
//                }
//            } else {
//                auto peer   = m_peers->get(event->stream);
//                peer->event = event;
////                LOGT << "push by acceptor cached peer fd " <<
////                    peer->event->stream->getID() << " event " << event->type;
//                m_worker_pool_peer_handler->push(peer);
//            }
//        }
//    }
//}


//void CSocketServerAcceptor::initialize() {
//    m_socket->initialize();
//    LOGT << "acceptor socket fd is " << m_socket->getID();
//    m_listener->initialize();
//    m_listener->add(m_socket);
//    m_worker_pool_peer_handler->initialize();
//}


//void CSocketServerAcceptor::finalize() {
//    m_listener->finalize();
//    m_socket->finalize();
//    m_worker_pool_peer_handler->finalize();
//}


} // implementation
} // net
} // io
} // iridium
