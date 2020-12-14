#ifndef HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
#define HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA


#include "iridium/io/stream_pool.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/worker_pool.h"

#include <map>
#include <list>


namespace iridium {
namespace io {
namespace implementation {


class CStreamPool: public IStreamPool {
public:
    DEFINE_IMPLEMENTATION(CStreamPool)
    CStreamPool();

    void add(IStreamPort::TSharedPtr const &stream, protocol::IProtocolHandler::TSharedPtr const &protocol_handler) override;
    void del(IStream::TSharedPtr     const &stream) override;

    void initialize()   override;
    void finalize()     override;

private:
    // io listener thread
    class CListenerHandler:
        public threading::IRunnable,
        public IStreamPool,
        protected threading::CSynchronized
    {
    public:
        DEFINE_IMPLEMENTATION(CListenerHandler)
        CListenerHandler();

        void    add(IStreamPort::TSharedPtr  const &stream, protocol::IProtocolHandler::TSharedPtr const &protocol_handler) override;
        void    add(IStream::TConstSharedPtr const &stream, protocol::IProtocolHandler::TSharedPtr const &protocol_handler);
        void    del(IStream::TSharedPtr const &stream) override;

        void    initialize()   override;
        void    finalize()     override;

        void    run(std::atomic<bool> &is_running) override;

    private:
        struct TransmitterHandler {
            DEFINE_CREATE(TransmitterHandler)
            Event::TSharedPtr                       event;
            protocol::IProtocolHandler::TSharedPtr  protocol_handler;
            io::ITransmitter::TSharedPtr            transmitter;
        };

        // streams threads
        class CStreamHandler: public threading::IWorkerHandler<TransmitterHandler::TSharedPtr> {
        public:
            DEFINE_IMPLEMENTATION(CStreamHandler)
            CStreamHandler(CListenerHandler &listener_handler);

            void    initialize  () override;
            void    finalize    () override;
            TItems  handle      (TItems const &stream_handler) override;
        private:
            CListenerHandler       &m_listener_handler;
        };

        std::map<int, protocol::IProtocolHandler::TSharedPtr>
                                    m_map_stream_protocol;
        threading::IWorkerPool<TransmitterHandler::TSharedPtr>::TSharedPtr
                                    m_worker_pool;
    };

    IStreamPool::TSharedPtr         m_runnable;
    threading::IThread::TSharedPtr  m_thread;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
