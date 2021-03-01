#include "server.h"

#include "iridium/io/implementation/stream_pool.h"
#include "iridium/io/implementation/listener.h"
#include "iridium/io/net/implementation/socket_acceptor.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/pattern/implementation/initializer.h"


using iridium::io::implementation::CStreamPool;
using iridium::io::implementation::CListener;
using iridium::io::net::implementation::CSocketAcceptor;
using iridium::threading::implementation::CThread;
using iridium::pattern::implementation::CInitializer;


#include "iridium/io/implementation/pipe.h"
#include "socket_acceptor.h"
#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CAcceptorSession: public protocol::ISession {
public:
    DEFINE_IMPLEMENTATION(CAcceptorSession)

    class CFakePipe: public io::implementation::CPipe {
    public:
        DEFINE_IMPLEMENTATION(CFakePipe)
        CFakePipe() = default;
        bool transmit(IListener::Event::TConstSharedPtr const &) override {
            return true;
        };
    };

    CAcceptorSession(
        URL                             const &url,
        IPeerSessionFactory::TSharedPtr const &session_factory,
        IStreamPool::TSharedPtr         const &stream_pool)
    :
        m_url               (url),
        m_session_factory   (session_factory),
        m_stream_pool       (stream_pool)
    {}

    IStream::TSharedPtr getStream(IListener::TSharedPtr const &listener) override {
        LOGT << "create acceptor";
        m_socket = CSocketAcceptor::create(m_url, listener);
        m_socket->initialize();
        return m_socket;
    }

    IPipe::TSharedPtr getPipe(IListener::Event::TConstSharedPtr const &event) override {
        if(!m_pipe) {
            LOGT << "init acceptor";
            m_pipe = CFakePipe::create();
        }

        if (event->stream == m_socket) {
            LOGT << "accepting";
            while (auto peer = m_socket->accept()) {
                m_stream_pool->add(m_session_factory->createSession(peer));
            }
        }

        return m_pipe;
    }

private:
    URL                             m_url;
    IPeerSessionFactory::TSharedPtr m_session_factory;
    IStreamPool::TSharedPtr         m_stream_pool;
    IPipe::TSharedPtr               m_pipe;
    ISocketAcceptor::TSharedPtr     m_socket;
};



CServer::CServer(URL const &url, IPeerSessionFactory::TSharedPtr const &session_factory)
:
    m_url               (url),
    m_session_factory   (session_factory),
    m_stream_pool       (CStreamPool::create())
{}


void CServer::initialize() {
    m_stream_pool->initialize();
    m_stream_pool->add(CAcceptorSession::create(m_url, m_session_factory, m_stream_pool));
}


void CServer::finalize() {
    m_stream_pool->finalize();
//    m_stream_pool->del();
}


} // implementation
} // net
} // io
} // iridium
