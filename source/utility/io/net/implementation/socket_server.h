#ifndef HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2
#define HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2


#include "socket.h"

#include "utility/io/net/url.h"
#include "utility/pattern/initializable.h"
#include "utility/io/protocol/protocol_factory.h"
#include "utility/threading/worker_pool.h"
#include "utility/threading/thread.h"
#include "utility/threading/runnable.h"
#include "utility/threading/synchronized.h"
#include "utility/io/listener.h"

#include <map>


namespace utility {
namespace io {
namespace net {
namespace implementation {


class CSocketServer: public pattern::IInitializable {
public:
    DEFINE_IMPLEMENTATION(CSocketServer)

    CSocketServer(
        URL const &url,
        int const &threads_count,
        protocol::IProtocolFactory::TSharedPtr const &protocol_factory);

    void initialize() override;
    void finalize() override;

private:
    class CAcceptor: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CAcceptor)

        CAcceptor(
            URL const &url,
            int const &threads_count,
            protocol::IProtocolFactory::TSharedPtr const &protocol_factory);

        void initialize()   override;
        void finalize()     override;
        void run(std::atomic<bool> &is_running) override;
    private:
        ISocket::TSharedPtr         m_socket;
        IListener::TSharedPtr       m_listener;
        threading::IWorkerPool <IListener::Event::TSharedPtr> ::TSharedPtr
                                    m_worker_pool;
    };

    class StreamLocker: public threading::Synchronized {
    public:
        StreamLocker();
        IStream::TSharedPtr get(int const &id);
        void set(int const &id, IStream::TSharedPtr &stream);
    private:
        std::map<int, IStream::TSharedPtr> m_map_id_stream;
    };

    class CSocketHandler:
        public threading::IWorkerHandler <IListener::Event::TSharedPtr>
    {
    public:
        DEFINE_IMPLEMENTATION(CSocketHandler)

        CSocketHandler(protocol::IProtocol::TSharedPtr const &protocol);

        void    initialize()    override;
        void    finalize()      override;
        TItems  handle(TItems const &item) override;

    private:
        class StreamHandler {
        public:
            DEFINE_SMART_PTR(StreamHandler)
            DEFINE_CREATE   (StreamHandler)
            StreamHandler(
                protocol::IProtocol::TSharedPtr const &protocol,
                IStream::TSharedPtr             const &stream
            );
           ~StreamHandler() = default;

            bool handle(IListener::Event const &event);
        private:
            protocol::IProtocol::TSharedPtr m_protocol;
            IStream::TSharedPtr             m_stream;
        };

        std::map<int, StreamHandler::TSharedPtr>    m_map_id_stream;
        protocol::IProtocol::TSharedPtr             m_protocol;
    };

    threading::IThread::TSharedPtr          m_thread_acceptor;
    protocol::IProtocolFactory::TSharedPtr  m_protocol_factory;
};


} // implementation
} // net
} // io
} // utility


#endif // HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2
