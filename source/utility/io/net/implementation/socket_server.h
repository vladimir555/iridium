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
#include "utility/io/stream_context.h"

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

        class StreamStorage: private threading::Synchronized {
        public:
            DEFINE_CREATE(StreamStorage)
            StreamStorage();
           ~StreamStorage() = default;

            StreamContext::TSharedPtr get(int const &id);
            void set(int const &id, StreamContext::TSharedPtr &stream_context);
        private:
            std::map<int, StreamContext::TSharedPtr> m_map_id_stream_context;
        };


        class CSocketHandler: public threading::IWorkerHandler <IListener::Event::TSharedPtr> {
        public:
            DEFINE_IMPLEMENTATION(CSocketHandler)

            CSocketHandler(protocol::IProtocol::TSharedPtr const &protocol, StreamStorage::TSharedPtr const &stream_storage);

            void    initialize()    override;
            void    finalize()      override;
            TItems  handle(TItems const &item) override;

        private:
            protocol::IProtocol::TSharedPtr     m_protocol;
            StreamStorage::TSharedPtr           m_stream_storage;
        };


        CAcceptor(
            URL const &url,
            int const &threads_count,
            protocol::IProtocolFactory::TSharedPtr const &protocol_factory);

        void initialize()   override;
        void finalize()     override;
        void run(std::atomic<bool> &is_running) override;
    private:
        IListener::TSharedPtr                   m_listener;
        ISocket::TSharedPtr                     m_socket;
        threading::IWorkerPool <IListener::Event::TSharedPtr> ::TSharedPtr
                                                m_worker_pool;
        protocol::IProtocolFactory::TSharedPtr  m_protocol_factory;
        StreamStorage::TSharedPtr               m_stream_storage;
    };


    threading::IThread::TSharedPtr              m_thread_acceptor;
};


} // implementation
} // net
} // io
} // utility


#endif // HEADER_SOCKET_SERVER_260D2BDD_82F1_424A_A68A_7BD30F4A00A2
