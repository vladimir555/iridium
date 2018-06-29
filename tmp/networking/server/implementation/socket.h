#ifndef HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
#define HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78


#include "utility/net/server/socket.h"
#include "utility/net/url.h"
#include "utility/net/socket.h"
#include "utility/net/socket_handler.h"

#include "utility/threading/thread.h"
#include "utility/threading/worker_pool.h"
#include "utility/threading/implementation/mutex.h"
#include "utility/threading/implementation/runnuble.h"

#include "utility/protocol/protocol.h"

#include <list>
#include <map>
#include <memory>
#include <chrono>


namespace utility {
namespace net {
namespace server {
namespace implementation {


class CSocket:
    public ISocket,
    public std::enable_shared_from_this<CSocket>
{
public:
    DEFINE_SMART_PTR(CSocket)
    DEFINE_IMPLEMENTATION(CSocket)
    CSocket(URL const &url, protocol::IProtocol::TSharedPtr const &protocol, int const &count);

    void initialize() override;
    void finalize() override;

private:
    class CAcceptor: public threading::implementation::CRunnuble {
    public:
        DEFINE_IMPLEMENTATION(CAcceptor)
        CAcceptor(URL const &url, utility::protocol::IProtocol::TSharedPtr const &protocol, int const &count);

        void initialize() override;
        void finalize() override;
        void run() override;

    private:

        // todo: not synchronized protocol for every thread
        class SynchronizedProtocolHandler:
            public threading::implementation::CMutex,
            public utility::protocol::IProtocol
        {
        public:
            DEFINE_IMPLEMENTATION(SynchronizedProtocolHandler)
            SynchronizedProtocolHandler(utility::protocol::IProtocol::TSharedPtr const &protocol);

            utility::protocol::IPacket::TSharedPtr exchange(utility::protocol::IPacket::TSharedPtr const &packet) override;

        private:
            utility::protocol::IProtocol::TSharedPtr m_protocol;
        };

        // thread
        class CEventsHandler: public TEventsHandler {
        public:
            DEFINE_IMPLEMENTATION(CEventsHandler)
            CEventsHandler(protocol::IProtocol::TSharedPtr const &protocol);

            TItems handle(TItems const &items) override;

            void initialize() override;
            void finalize() override;
        private:
            // sockets map
            std::map<URL, ISocketHandler::TSharedPtr>   m_map_url_socket;
            protocol::IProtocol::TSharedPtr             m_protocol;
        };

        utility::protocol::IProtocol::TSharedPtr    m_protocol;
        net::ISocket::TSharedPtr             m_socket;
        TEventsWorkerPool::TSharedPtr               m_worker_pool;
    };

    threading::IRunnable::TSharedPtr    m_acceptor_runnuble;
    threading::IThread::TSharedPtr      m_acceptor_thread;
};


} // implementation
} // server
} // net
} // utility


#endif // HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
