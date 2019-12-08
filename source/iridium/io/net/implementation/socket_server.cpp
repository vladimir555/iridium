/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "socket_server.h"


#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/threading/implementation/recursive_mutex.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/synchronized_scope.h"

#include "iridium/io/implementation/listener.h"
#include "iridium/io/implementation/transmitter.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/io/implementation/stream_reader_list.h"
#include "iridium/io/protocol/http/request.h"
#include "iridium/io/protocol/http/response.h"
#include "iridium/io/protocol/http/implementation/content_storage.h"

#include "iridium/pattern/implementation/initializer.h"

#include "iridium/parsing/implementation/parser_http.h"

#include <string>


using iridium::threading::implementation::CRecursiveMutex;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CAsyncQueue;
using iridium::threading::IWorkerPool;
using iridium::threading::IJob;
using iridium::io::implementation::CListener;
//using iridium::io::implementation::CStreamProxy;
//using iridium::io::implementation::CTransmitter;
using iridium::io::IListener;
using iridium::io::IStream;
using iridium::io::implementation::CStreamReaderBuffer;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::io::implementation::CStreamReaderList;
using iridium::io::implementation::CTransmitter;
//using iridium::io::protocol::IProtocol;
using iridium::io::protocol::IProtocolFactory;
//using iridium::io::protocol::http::implementation::CProtocol;
using iridium::io::protocol::http::implementation::CContentStorage;
using iridium::convertion::convert;
using iridium::pattern::implementation::CInitializer;
using iridium::parsing::implementation::CNode;
using iridium::parsing::implementation::CHTTPParser;

using std::map;
using std::string;


size_t const DEFAULT_BUFFER_SIZE = 4;


namespace iridium {
namespace io {
namespace net {
namespace implementation {

    
//    class Peer {
//    public:
//        Buffer::TSharedPtr recive(Buffer::TSharedPtr const &stop_symbol, size_t const &min_size = 0, size_t const &max_size = 0);
//        void send(Buffer::TSharedPtr const &buffer);
//    };
    
//    class IProtocol {
//    public:
//        DEFINE_INTERFACE(IProtocol)
//        class ISwitch {
//        public:
//            DEFINE_INTERFACE(ISwitch)
//            virtual void set(IStreamReader::TSharedPtr const &reader) = 0;
//            virtual void set(IStreamWriter::TSharedPtr const &writer) = 0;
//        };
//
//        enum TAction {
//            CLIENT_READ,
//            SERVER_READ,
//            STOP
//        };
//
//        virtual TAction update(ISwitch::TSharedPtr const protocol_switch);
//    };
//
//    class CSplitter: public IPort {
//    public:
//        DEFINE_IMPLEMENTATION(CSplitter)
//        CSplitter(IStreamReader::TSharedPtr const &reader, IStreamWriter::TSharedPtr const &writer);
//        void    initialize()    override;
//        void    finalize()      override;
//        Buffer  read (size_t const &size)    override;
//        size_t  write(Buffer const &buffer)  override;
//    };
//
//    class Transmitter {
//    public:
//        DEFINE_CREATE(Transmitter)
//        Transmitter(
//            IPort::TSharedPtr const &server,
//            IPort::TSharedPtr const &client);
//       ~Transmitter();
//        void transmit(bool const &is_transmit_to_client);
//    };
    

CSocketServer::CSocketServer(
    URL     const &url,
    size_t  const &threads_count,
    IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_thread_acceptor(CThread::create(CAcceptor::create(url, threads_count, protocol_factory), "socket_acceptor"))
{}


void CSocketServer::initialize() {
    m_thread_acceptor->initialize();
}


void CSocketServer::finalize() {
    m_thread_acceptor->finalize();
}


CSocketServer::CAcceptor::CAcceptor(
    URL     const &url,
    size_t  const &threads_count,
    IProtocolFactory::TSharedPtr const &protocol_factory)
:
    m_listener          (CListener::create()),
    m_streams_to_delete (CAsyncQueue<IStreamPort::TSharedPtr>::create()),
    m_socket            (CSocket::create(url, true)),
    m_peers             (Peers::create(protocol_factory, m_streams_to_delete))
{
    IWorkerPool<io::Event::TSharedPtr>::TWorkerHandlers handlers;
    for (size_t i = 0; i < threads_count; i++) {
        // todo: !!!!! factory of factory thread saving
        auto handler = CIOEventHandler::create(m_peers);
        handlers.push_back(handler);
    }

    m_worker_pool_peer_handler  = CWorkerPool<io::Event::TSharedPtr>::create("socket_handler", handlers);
    m_initializer               = CInitializer::create(m_listener, m_socket, m_worker_pool_peer_handler);
}


void CSocketServer::CAcceptor::initialize() {
    m_initializer->initialize();
}


void CSocketServer::CAcceptor::finalize() {
    m_initializer->finalize();
}


void CSocketServer::CAcceptor::run(std::atomic<bool> &is_running) {
    m_listener->add(m_socket);
    while (is_running) {
//        LOGT << "sleep";
        threading::sleep(500);
        for (auto const &stream: m_streams_to_delete->pop(false))
            m_listener->del(stream);
        // all fds are unique from listener
        for (auto const &event: m_listener->wait()) {
            if (event->stream->getID() == m_socket->getID()) {
                LOGT << "try accept";
                while (auto peer = m_socket->accept())
                    m_listener->add(peer);
            } else
                m_worker_pool_peer_handler->push(event);
        }
    }
}


CSocketServer::CAcceptor::Peers::Peers(
    IProtocolFactory::TSharedPtr                                const &protocol_factory,
    threading::IAsyncQueue<IStreamPort::TSharedPtr>::TSharedPtr const &streams_to_delete)
:
    Synchronized        (threading::implementation::CMutex::create()),
    m_protocol_factory  (protocol_factory),
    m_streams_to_delete (streams_to_delete)
{}


bool CSocketServer::CAcceptor::Peers::handle(io::Event::TSharedPtr const &event) {
    TPeer::TSharedPtr peer;

    {
        LOCK_SCOPE_FAST
        auto i = m_map_stream_peer.find(event->stream);
        if (i == m_map_stream_peer.end()) {
            LOGT << "peer create";
            peer = TPeer::create();
            peer->protocol_handler  = m_protocol_factory->createProtocolHandler();
            peer->transmitter       = CTransmitter::create(DEFAULT_BUFFER_SIZE);

            m_map_stream_peer[event->stream] = peer;
        } else {
            LOGT << "peer get";
            // m_map_stream_peer[event->stream] == nullptr means that peer in processing
            peer = std::move(i->second);
            if (!peer)
                return false; // ----->
        }
    }

    bool is_continue    = false;
    bool is_transmitted = false;
    try {
        LOGT << "peer protocol update";
        is_continue = peer->protocol_handler->update(peer->transmitter, event);
        LOGT << "peer protocol update result " << is_continue;
        if (is_continue) {
            LOGT << "peer transmit " << is_continue;
            is_transmitted = peer->transmitter->transmit();
            LOGT << "peer transmit result " << is_transmitted;
        }
    } catch (std::exception const &e) {
        LOGE << "peer error: " << e.what();
        is_continue = false;
    }

    if (!is_continue) {
        LOGT << "peer delete";
        m_streams_to_delete->push(event->stream);
        try {
            event->stream->finalize();
        } catch (std::exception const &e) {
            LOGE << "peer error: " << e.what();
        }
    }

    {
        LOCK_SCOPE_FAST
        m_map_stream_peer[event->stream] = std::move(peer);
        return is_continue && is_transmitted; // ----->
    }
}


CSocketServer::CAcceptor::CIOEventHandler::CIOEventHandler(Peers::TSharedPtr const &peers)
:
    m_peers(peers)
//    m_protocol_factory  (protocol_factory),
//    m_peers             (Peers::create())
//    m_buffer_size       (5),
//    m_parser            (CHTTPParser::create()),
//    m_content_storage   (CContentStorage::create("."))
{}


void CSocketServer::CAcceptor::CIOEventHandler::initialize() {}


void CSocketServer::CAcceptor::CIOEventHandler::finalize() {}

    
//bool CSocketServer::CAcceptor::CIOEventHandler::transmit(
//    IStreamReader::TSharedPtr const &reader,
//    IStreamWriter::TSharedPtr const &writer)
//{
//    auto buffer = reader->read(m_buffer_size);
//
//    LOGT << "read  '" << buffer << "' buffer.size = " << buffer.size();
//
//    if (buffer.empty())
//        return false; // ----->
//
//    auto size = writer->write(buffer);
//
//    LOGT << "write '" << buffer << "'";
//
//    if (size < buffer.size())
//        throw std::runtime_error(
//            "stream proxy error: write size "   + convert<string>(size) +
//            " < read size "                     + convert<string>(buffer.size())); // ----->
//    else
//        return true; // ----->
//}


CSocketServer::CAcceptor::CIOEventHandler::TItems
CSocketServer::CAcceptor::CIOEventHandler::handle(TItems const &events) {
//    LOGT << "sleep";
    threading::sleep(500);
    
    TItems events_;

    for (auto event: events) {
        LOGT << "event fd " << event->stream->getID() << " " << event->type;
        if (m_peers->handle(event))
            events_.push_back(event);
    }

    return events_; // ----->

//    for (auto event: events) {
//        LOGT << "event fd " << event->stream->getID() << " " << event->type;
//        auto peer = m_peers->holdPeer(event->stream);
//        // todo: handling buffer overflow and eof on transmitting
//        static int i =0;
//        if (peer) {
//            try {
//                LOGT << "protocol handle begin";
//                peer->is_continue = peer->protocol_handler->update(peer->transmitter, event);
//                LOGT << "protocol handle end";

//                LOGT << "transmit begin";
//                if (peer->transmitter->transmit())
//                    events_.push_back(event);
////                else {
////                    // stop streaming on eof and protocol finished
////                    if (!peer->is_continue) {
////                        LOGT << "1 close";
////                        event->stream->finalize();
////                        LOGT << "2 close";
////                    }
////                }
//                i++;
//                if (i == 5) // todo: rm
//                    throw std::runtime_error("fake exception");
//                LOGT << "transmit end";
//            } catch (std::exception const &e) {
//                LOGE << "peer error: " << e.what();
//                peer->is_continue = false;
//            }
//            if (!peer->is_continue)
//                m_streams_to_delete->push(event->stream);
//            m_peers->releasePeer(event->stream);
//        } else {
//            LOGE << "peer is null";
//        }
//    }
    
//    for (auto event: events) {
//        LOGT << "next";
//        auto id     = event->stream->getID();
//        auto peer   = m_peers->get(id);
//
//        LOGT << "event: id = " << id << " type " << event->type;
//
//        try {
//            bool result = false;
//            if (peer.client_stream) {
//                if (peer.server_stream_writer) {
//                    LOGT << "continue read client";
//                    result = transmit(peer.client_stream, peer.server_stream_writer);
//                    LOGT << 1;
//
//                    if (peer.buffer) {
//                        LOGT << "client request buffer:\n'" << *peer.buffer << "'";
//                    }
//
//                    if (result && peer.buffer &&
//                        peer.buffer->size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
//                        string(peer.buffer->end() - 4, peer.buffer->end()) == "\r\n\r\n")
//                    {
//                        protocol::http::request ::THttp request     (m_parser->parse(convert<string>(*peer.buffer)));
//                        protocol::http::response::THttp response    (CNode::create("http"));
//
//                        LOGT << "parsed client request:\n" << request.getNode();
//
//                        auto content = m_content_storage->getContent("." + request.Message.get().uri);
//
//                        response.Headers.ContentLength = response.Body.get().size() + content->getSize();
//
//                        auto stream_list    = CStreamReaderList::create();
//                        auto header_buffer  = Buffer::create(m_parser->compose(response.getNode()));
//
//                        LOGT << "header buffer:\n"  << *header_buffer;
//                        LOGT << "header uri: '"     << request.Message.get().uri << "'";
//
//                        stream_list->add(CStreamReaderBuffer::create(header_buffer));
//                        stream_list->add(content);
//
//                        peer.server_stream_writer.reset();
//                        peer.server_stream_reader = stream_list;
//
//                        LOGT << "begin write client";
//                    }
//                }
//
//                if (peer.server_stream_reader) {
//                    LOGT << "continue write client";
//                    result = transmit(peer.server_stream_reader, peer.client_stream);
//                    if (result)
//                        events_.push_back(event);
//                }
//            } else {
//                LOGT << "begin read";
//                peer.buffer                 = Buffer::create();
//                peer.client_stream          = std::dynamic_pointer_cast<ISocket>(event->stream);
//                peer.server_stream_writer   = CStreamWriterBuffer::create(peer.buffer);
//                result = true;
//            }
//
//            if (result) {
//                m_peers->set(id, peer);
//            } else {
//                LOGT << "close client";
//                peer.client_stream->finalize();
//                m_peers->del(id);
//            }
//        } catch (std::exception const &e) {
//            LOGE << e.what() << ", close socket";
//            peer.client_stream->finalize();
//            m_peers->del(id);
//        }
//    }

//    return events_; // ----->
}


} // implementation
} // net
} // io
} // iridium
