#ifndef HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
#define HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA


#include "iridium/io/stream_pool.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/worker_pool.h"
#include "iridium/threading/synchronized_container.h"

#include "transmitter.h"

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
        protected threading::Synchronized
    {
    public:
        DEFINE_IMPLEMENTATION(CListenerHandler)
        CListenerHandler();

        void    add(IStreamPort::TSharedPtr const &stream, protocol::IProtocolHandler::TSharedPtr const &protocol_handler) override;
        void    del(IStream::TSharedPtr const &stream) override;

        void    initialize()   override;
        void    finalize()     override;

        void    run(std::atomic<bool> &is_running) override;

    private:
        typedef threading::SynchronizedContainer<IStream::TSharedPtr, ITransmitter::TSharedPtr> TTransmitters;

        class CTransmitterHandler:
            public threading::Synchronized,
            public CTransmitter,
            public std::enable_shared_from_this<CTransmitterHandler>
        {
        public:
            DEFINE_CREATE(CTransmitterHandler)
            CTransmitterHandler(protocol::IProtocolHandler::TSharedPtr const &protocol_handler, TTransmitters &transmitters);
            virtual ~CTransmitterHandler();

            void set(const IStreamReader::TSharedPtr &reader, const IStreamWriter::TSharedPtr &writer) override;
            bool transmit(const Event::TSharedPtr &event) override;

        private:
            TTransmitters          &m_transmitters;
            protocol::IProtocolHandler::TSharedPtr
                                    m_protocol_handler;

        };

        // streams threads
        class CStreamHandler: public threading::IWorkerHandler<Event::TSharedPtr> {
        public:
            DEFINE_IMPLEMENTATION(CStreamHandler)
            CStreamHandler(TTransmitters &transmitters);

            void    initialize  () override;
            void    finalize    () override;
            TItems  handle      (TItems const &events) override;
        private:
            TTransmitters          &m_transmitters;
        };

        TTransmitters               m_transmitters;
        threading::IWorkerPool<Event::TSharedPtr>::TSharedPtr
                                    m_worker_pool;
    };

    IStreamPool::TSharedPtr         m_runnable;
    threading::IThread::TSharedPtr  m_thread;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
