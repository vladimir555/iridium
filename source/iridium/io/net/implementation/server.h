#ifndef HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
#define HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC


#include "iridium/io/net/server.h"
#include "iridium/io/net/socket.h"

#include "iridium/io/stream_pool.h"
#include "iridium/io/protocol/protocol_factory.h"

#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CServer: public IServer {
public:
    DEFINE_IMPLEMENTATION(CServer)
    CServer(
        URL const &url,
        protocol::IProtocolFactory::TSharedPtr const &protocol_factory);

    void initialize()   override;
    void finalize()     override;

private:
    class CRunnable: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CRunnable)
        CRunnable(URL const &url, protocol::IProtocolFactory::TSharedPtr const &protocol_factory);

        void initialize()   override;
        void finalize()     override;

        void run(std::atomic<bool> &is_running) override;
    private:
        ISocketAcceptor::TSharedPtr             m_socket;
        IStreamPool::TSharedPtr                 m_stream_pool;
        protocol::IProtocolFactory::TSharedPtr  m_protocol_factory;
    };

    threading::IThread::TSharedPtr m_thread;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
