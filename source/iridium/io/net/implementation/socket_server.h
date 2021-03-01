// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2
#define HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2


#include "iridium/pattern/initializable.h"
#include "iridium/threading/thread.h"
#include "iridium/io/url.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


//class CSocketServer:
//    public pattern::IInitializable
//{
//public:
//    DEFINE_IMPLEMENTATION(CSocketServer)

//    CSocketServer(
//        URL     const &url,
//        protocol::IProtocolFactory::TSharedPtr const &protocol_factory,
//        size_t  const &threads_count);

//    void initialize()   override;
//    void finalize()     override;

//private:
//    threading::IThread::TSharedPtr m_thread_acceptor;
//};


} // implementation
} // net
} // io
} // iridium



//#include "socket.h"
//
//#include "iridium/threading/worker_pool.h"
//#include "iridium/threading/thread.h"
//#include "iridium/threading/runnable.h"
//#include "iridium/threading/synchronized.h"
//#include "iridium/threading/synchronized_container.h"
//#include "iridium/threading/async_queue.h"
//
//#include "iridium/pattern/initializable.h"
//
//#include "iridium/parsing/parser.h"
//
//#include "iridium/io/buffer.h"
//#include "iridium/io/net/url.h"
//#include "iridium/io/protocol/protocol_factory.h"
//#include "iridium/io/protocol/http/content_storage.h"
//#include "iridium/io/protocol/protocol_factory.h"
//#include "iridium/io/listener.h"
//
//#include <map>
//
//
//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {
//
//
//class CSocketServer: public pattern::IInitializable {
//public:
//    DEFINE_IMPLEMENTATION(CSocketServer)
//
//    CSocketServer(
//        URL     const &url,
//        size_t  const &threads_count,
//        protocol::IProtocolFactory::TSharedPtr const &protocol_factory);
//
//    void initialize()   override;
//    void finalize()     override;
//
//private:
//    class CAcceptor: public threading::IRunnable {
//    public:
//        DEFINE_IMPLEMENTATION(CAcceptor)
//
//        struct TPeer {
//            DEFINE_CREATE(TPeer)
//            bool                                    is_continue;
//            protocol::IProtocolHandler::TSharedPtr  protocol_handler;
//            ITransmitter::TSharedPtr                transmitter;
//        };
//
//        class Peers: public threading::Synchronized {
//        public:
//            DEFINE_CREATE(Peers)
//            Peers(
//                IListener::TSharedPtr                   const &listener,
//                protocol::IProtocolFactory::TSharedPtr  const &protocol_factory);
//            virtual ~Peers() = default;
//
//            TPeer::TSharedPtr   getPeer(IStream::TSharedPtr const &stream);
//            void                delPeer(IStream::TSharedPtr const &stream);
//            void                setPeer(IStream::TSharedPtr const &stream, TPeer::TSharedPtr const &peer);
//        private:
//            protocol::IProtocolFactory::TSharedPtr              m_protocol_factory;
//            std::map<IStream::TSharedPtr, TPeer::TSharedPtr>    m_map_stream_peer;
//            IListener::TSharedPtr                               m_listener;
//        };
//        
//        class CIOEventHandler: public threading::IWorkerHandler<io::Event::TSharedPtr> {
//        public:
//            DEFINE_IMPLEMENTATION(CIOEventHandler)
//
//            CIOEventHandler(Peers::TSharedPtr const &peers);
//
//            void    initialize()    override;
//            void    finalize()      override;
//            TItems  handle(TItems const &events) override;
//
//        private:
//            protocol::IProtocolFactory::TSharedPtr  m_protocol_factory;
//            Peers::TSharedPtr                       m_peers;
//        };
//
//        CAcceptor(
//            URL    const &url,
//            size_t const &threads_count,
//            protocol::IProtocolFactory::TSharedPtr const &protocol_factory);
//
//        void initialize()   override;
//        void finalize()     override;
//        void run(std::atomic<bool> &is_running) override;
//        
//    private:
//        IListener::TSharedPtr       m_listener;
//
//        ISocket::TSharedPtr         m_socket;
//        threading::IWorkerPool <Event::TSharedPtr>::TSharedPtr
//                                    m_worker_pool_peer_handler;
//        protocol::IProtocolFactory::TSharedPtr
//                                    m_protocol_factory;
//        
//        Peers::TSharedPtr           m_peers;
//        IInitializable::TSharedPtr  m_initializer;
//    };
//
//    threading::IThread::TSharedPtr  m_thread_acceptor;
//};
//
//
//} // implementation
//} // net
//} // io
//} // iridium


#endif // HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2
