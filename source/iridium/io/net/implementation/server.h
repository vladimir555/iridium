#ifndef HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
#define HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC


#include "iridium/io/server.h"
#include "iridium/io/stream_pool.h"
#include "iridium/io/protocol/session.h"
#include "iridium/io/protocol/peer_session_factory.h"
#include "iridium/io/net/socket.h"

#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CServer: public IServer {
public:
    DEFINE_IMPLEMENTATION(CServer)
    CServer(URL const &url, protocol::IPeerSessionFactory::TSharedPtr const &session_factory);

    void initialize()   override;
    void finalize()     override;

private:
    class CAcceptorSession: public protocol::ISession {
    public:
        DEFINE_IMPLEMENTATION(CAcceptorSession)
        class CFakePipe: public IPipe {
        public:
            DEFINE_IMPLEMENTATION(CFakePipe)
            CFakePipe() = default;
            IStreamReader::TConstSharedPtr getReader() const override;
            IStreamWriter::TConstSharedPtr getWriter() const override;
            void set(IStreamReader::TSharedPtr const &, IStreamWriter::TSharedPtr const &) override;
            bool transmit(IListener::Event::TConstSharedPtr const &) override;
        };

        CAcceptorSession(
            URL const &url,
            protocol::IPeerSessionFactory::TSharedPtr const &session_factory,
            IStreamPool::TSharedPtr const &stream_pool);

        IStream::TSharedPtr getStream(IListener::TSharedPtr const &listener) override;
        IPipe::TSharedPtr   getPipe(IListener::Event::TConstSharedPtr const &event) override;
    private:
        URL                             m_url;
        protocol::IPeerSessionFactory::TSharedPtr m_session_factory;
        IStreamPool::TSharedPtr         m_stream_pool;
        IPipe::TSharedPtr               m_pipe;
        ISocketAcceptor::TSharedPtr     m_socket;
    };

    IStreamPool::TSharedPtr         m_stream_pool;
    protocol::ISession::TSharedPtr  m_acceptor_session;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
