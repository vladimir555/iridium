/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "socket_server.h"


#include "socket_server_acceptor.h"

#include "iridium/threading/implementation/thread.h"


using iridium::threading::implementation::CThread;
using iridium::io::protocol::IProtocolFactory;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


CSocketServer::CSocketServer(
    URL     const &url,
    IProtocolFactory::TSharedPtr const &protocol_factory,
    size_t  const &threads_count)
:
    m_thread_acceptor(
        CThread::create(
            CSocketServerAcceptor::create(
                url, 
                protocol_factory, 
                threads_count),
            "socket_acceptor"))
{}


void CSocketServer::initialize() {
    m_thread_acceptor->initialize();
}


void CSocketServer::finalize() {
    m_thread_acceptor->finalize();
}


} // implementation
} // net
} // io
} // iridium


//#include "iridium/threading/implementation/thread.h"
//#include "iridium/threading/implementation/worker_pool.h"
//#include "iridium/threading/implementation/recursive_mutex.h"
//#include "iridium/threading/implementation/async_queue.h"
//#include "iridium/threading/synchronized_scope.h"
//
//#include "iridium/io/implementation/listener.h"
//#include "iridium/io/implementation/transmitter.h"
//#include "iridium/io/implementation/stream_buffer.h"
//#include "iridium/io/implementation/stream_reader_list.h"
//#include "iridium/io/protocol/http/request.h"
//#include "iridium/io/protocol/http/response.h"
//#include "iridium/io/protocol/http/implementation/content_storage.h"
//
//#include "iridium/pattern/implementation/initializer.h"
//#include "iridium/parsing/implementation/parser_http.h"
//
//#include <string>
//
//
//using iridium::threading::implementation::CRecursiveMutex;
//using iridium::threading::implementation::CThread;
//using iridium::threading::implementation::CWorkerPool;
//using iridium::threading::implementation::CMutex;
//using iridium::threading::implementation::CAsyncQueue;
//using iridium::threading::IWorkerPool;
//using iridium::threading::IJob;
//using iridium::io::implementation::CListener;
////using iridium::io::implementation::CStreamProxy;
////using iridium::io::implementation::CTransmitter;
//using iridium::io::IListener;
//using iridium::io::IStream;
//using iridium::io::implementation::CStreamReaderBuffer;
//using iridium::io::implementation::CStreamWriterBuffer;
//using iridium::io::implementation::CStreamReaderList;
//using iridium::io::implementation::CTransmitter;
////using iridium::io::protocol::IProtocol;
//using iridium::io::protocol::IProtocolFactory;
////using iridium::io::protocol::http::implementation::CProtocol;
//using iridium::io::protocol::http::implementation::CContentStorage;
//using iridium::convertion::convert;
//using iridium::pattern::implementation::CInitializer;
//using iridium::parsing::implementation::CNode;
//using iridium::parsing::implementation::CHTTPParser;
//
//using std::map;
//using std::string;
//
//
//size_t const DEFAULT_BUFFER_SIZE = 8192;
//
//
//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {
//
//
//
//class AsyncListener : public IListener {
//public:
//    DEFINE_IMPLEMENTATION(AsyncListener)
//    AsyncListener(IListener::TSharedPtr const &listener)
//    :
//        m_listener(listener),
//        m_queue_streams_to_del(CAsyncQueue<IStream::TSharedPtr>::create()),
//        m_queue_streams_to_add(CAsyncQueue<IStream::TSharedPtr>::create())
//    {}
//
//    void initialize() override {
//        m_listener->initialize();
//    }
//
//    void finalize() override {
//        m_listener->finalize();
//    }
//
//    void add(IStream::TSharedPtr const &stream) override {
//        m_queue_streams_to_add->push(stream);
//    }
//
//    void del(IStream::TSharedPtr const &stream) override {
//        m_queue_streams_to_del->push(stream);
//    }
//
//    TEvents wait() override {
//        for (auto const &stream : m_queue_streams_to_del->pop(false))
//            m_listener->del(stream);
//        for (auto const &stream : m_queue_streams_to_add->pop(false))
//            m_listener->add(stream);
//
//        return m_listener->wait(); // ----->
//    }
//
//private:
//    IListener::TSharedPtr m_listener;
//    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_queue_streams_to_del;
//    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_queue_streams_to_add;
//};
//
//    
//CSocketServer::CSocketServer(
//    URL     const &url,
//    size_t  const &threads_count,
//    IProtocolFactory::TSharedPtr const &protocol_factory)
//:
//    m_thread_acceptor(CThread::create(CAcceptor::create(url, threads_count, protocol_factory), "socket_acceptor"))
//{}
//
//
//void CSocketServer::initialize() {
//    m_thread_acceptor->initialize();
//}
//
//
//void CSocketServer::finalize() {
//    m_thread_acceptor->finalize();
//}
//
//
//CSocketServer::CAcceptor::CAcceptor(
//    URL     const &url,
//    size_t  const &threads_count,
//    IProtocolFactory::TSharedPtr const &protocol_factory)
//:
//    m_listener  (AsyncListener::create(CListener::create())),
//    m_socket    (CSocket::create(url, true)),
//    m_peers     (Peers::create(m_listener, protocol_factory))
//{
//    IWorkerPool<io::Event::TSharedPtr>::TWorkerHandlers handlers;
//    for (size_t i = 0; i < threads_count; i++) {
//        // todo: !!!!! factory of factory thread saving
//        auto handler = CIOEventHandler::create(m_peers);
//        handlers.push_back(handler);
//    }
//
//    m_worker_pool_peer_handler  = CWorkerPool<io::Event::TSharedPtr>::create("socket_handler", handlers);
//    m_initializer               = CInitializer::create(m_listener, m_socket, m_worker_pool_peer_handler);
//}
//
//
//void CSocketServer::CAcceptor::initialize() {
//    m_initializer->initialize();
//}
//
//
//void CSocketServer::CAcceptor::finalize() {
//    m_initializer->finalize();
//}
//
//
//void CSocketServer::CAcceptor::run(std::atomic<bool> &is_running) {
//    m_listener->add(m_socket);
//    while (is_running) {
////        LOGT << "sleep";
////        threading::sleep(500);
//
//        for (auto const &event: m_listener->wait()) {
//            if (event->stream->getID() == m_socket->getID()) {
////                LOGT << "try accept";
//                while (auto peer = m_socket->accept())
//                    m_listener->add(peer);
//            } else
//                m_worker_pool_peer_handler->push(event);
//        }
//    }
//}
//
//
//CSocketServer::CAcceptor::Peers::Peers(
//    IListener::TSharedPtr           const &listener,
//    IProtocolFactory::TSharedPtr    const &protocol_factory)
//:
//    Synchronized        (threading::implementation::CMutex::create()),
//    m_protocol_factory  (protocol_factory),
//    m_listener          (listener)
//{}
//
//
//CSocketServer::CAcceptor::TPeer::TSharedPtr
//CSocketServer::CAcceptor::Peers::getPeer(IStream::TSharedPtr const &stream) {
//    LOCK_SCOPE_FAST
//
//    auto peer = m_map_stream_peer[stream];
//    if (!peer) {
//        peer = TPeer::create();
//        peer->protocol_handler      = m_protocol_factory->createProtocolHandler();
//        peer->transmitter           = CTransmitter::create(m_listener, DEFAULT_BUFFER_SIZE);
//        peer->transmitter->setReader(std::dynamic_pointer_cast<IStreamPort>(stream));
//        m_map_stream_peer[stream]   = peer;
//        LOGT << "fd " << stream->getID() << " new";
//    } else
//        LOGT << "fd " << stream->getID();
//
//    return peer; // ----->
//}
//
//
//void CSocketServer::CAcceptor::Peers::delPeer(IStream::TSharedPtr const &stream) {
//    LOCK_SCOPE_FAST
//
//    m_map_stream_peer.erase(stream);
//    LOGT << "stream: " << stream->getID();
//}
//
//
//void CSocketServer::CAcceptor::Peers::setPeer(IStream::TSharedPtr const &stream, TPeer::TSharedPtr const &peer) {
//    LOCK_SCOPE_FAST
//
//    if (stream->getID() == 0)
//        return; // ----->
//
//    m_map_stream_peer[stream] = peer;
//    LOGT << "stream: " << stream->getID();
//}
//
//
//CSocketServer::CAcceptor::CIOEventHandler::CIOEventHandler(Peers::TSharedPtr const &peers)
//:
//    m_peers(peers)
//{}
//
//
//void CSocketServer::CAcceptor::CIOEventHandler::initialize() {}
//
//
//void CSocketServer::CAcceptor::CIOEventHandler::finalize() {}
//
//
//CSocketServer::CAcceptor::CIOEventHandler::TItems
//CSocketServer::CAcceptor::CIOEventHandler::handle(TItems const &events) {
////    LOGT << "sleep";
////    threading::sleep(500);
//    
//
//    // 1. protocol: transmitter -> exchange streams, transmitter -> set source stream
//    // 2. transmitter: on set stream add fd to listener
//    // 3. transmitter: on read event read, on write event write
//
//    TItems events_;
//
//    for (auto event: events) {
//        //LOGT << "event fd " << event->stream->getID() << " " << event->type;
//        // todo: !!! set peer for source stream after protocol
//        auto peer = m_peers->getPeer(event->stream);
//        // todo: handling buffer overflow and eof on transmitting
//        try {
////            LOGT << "protocol handle begin";
//            peer->is_continue = peer->protocol_handler->update(peer->transmitter, event);
//
//            // one transmitter for both streams
//
//            // todo: rm
//            //auto stream_id =
//            //    event->stream ?
//            //    event->stream->getID() : 0;
//
//            //auto reader_id =
//            //    peer->transmitter->getReader() ?
//            //    peer->transmitter->getReader()->getID() : 0;
//
//            //if  (reader_id != 0 && reader_id != stream_id)
//            //    m_peers->setPeer(peer->transmitter->getReader(), peer);
//
//            //m_peers->setPeer(peer->transmitter->getReader(), peer);
//
//            // todo: rm
//            //if (peer->transmitter->getWriter()->getID() != event->stream->getID() &&
//            //    peer->transmitter->getWriter()->getID() != 0)
//            //    m_peers->setPeer(peer->transmitter->getWriter(), peer);
//
////            LOGT << "protocol handle end";
//
////            LOGT << "transmit begin";
//            // todo: !!! fix peer error: transmitter error: write size 4959 < read size 8192
//            LOGT;
//            threading::sleep(500);
//            if (peer->transmitter->transmit(event))
//                events_.push_back(event);
//            else {
//                // stop streaming on eof and protocol finished
//                if (!peer->is_continue) {
//                    LOGT << "fd " << event->stream->getID() << " stop peer";
//                    m_peers->delPeer(event->stream);
//                    // todo: del by both streams
//                }
//            }
////            LOGT << "transmit end";
//        } catch (std::exception const &e) {
//            LOGE << "peer error: " << e.what();
//            m_peers->delPeer(event->stream);
//        }
//    }
//
//    return events_; // ----->
//}
//
//
//} // implementation
//} // net
//} // io
//} // iridium
