#ifndef HEADER_SOCKET_SERVER_ACCEPTOR_0DE27660_B36E_43E9_ADA8_1325F3B11AA8
#define HEADER_SOCKET_SERVER_ACCEPTOR_0DE27660_B36E_43E9_ADA8_1325F3B11AA8


#include <map>


#include "iridium/threading/runnable.h"
#include "iridium/threading/worker_pool.h"
#include "iridium/io/listener.h"
#include "iridium/io/net/socket.h"
#include "iridium/io/url.h"

#include "peers.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


//class CSocketServerAcceptor: public threading::IRunnable {
//public:
//    DEFINE_IMPLEMENTATION(CSocketServerAcceptor)

//    CSocketServerAcceptor(
//        URL                                     const &url,
//        protocol::IProtocolFactory::TSharedPtr  const &protocol_factory,
//        size_t                                  const &threads_count
//    );

//    void run(std::atomic<bool> &is_running) override;

//    void initialize()   override;
//    void finalize()     override;

//private:
//    protocol::IProtocolFactory::TSharedPtr
//                            m_protocol_factory;
//    IListener::TSharedPtr   m_listener;
//    ISocket::TSharedPtr     m_socket;
//    Peers::TSharedPtr       m_peers;

//    threading::IWorkerPool<Peer::TSharedPtr>::TSharedPtr
//                            m_worker_pool_peer_handler;
//};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_SERVER_ACCEPTOR_0DE27660_B36E_43E9_ADA8_1325F3B11AA8
