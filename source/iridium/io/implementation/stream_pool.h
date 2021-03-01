#ifndef HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
#define HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA


#include "iridium/io/stream_pool.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/worker_pool.h"
#include "iridium/threading/synchronized_container.h"
#include "iridium/threading/async_queue.h"

#include "pipe.h"

#include <map>
#include <list>


namespace iridium {
namespace io {
namespace implementation {


class CStreamPool: public IStreamPool {
public:
    DEFINE_IMPLEMENTATION(CStreamPool)
    CStreamPool();

    void add(protocol::ISession::TSharedPtr const &session) override;
    void del(protocol::ISession::TSharedPtr const &session) override;

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
        CListenerHandler(IListener::TSharedPtr const &listener);

        void add(protocol::ISession::TSharedPtr const &session) override;
        void del(protocol::ISession::TSharedPtr const &session) override;

        void initialize()   override;
        void finalize()     override;

        void run(std::atomic<bool> &is_running) override;

    private:

        struct Sessions {
            DEFINE_CREATE(Sessions)
            Sessions();
            threading::SynchronizedContainer<IStream::TConstSharedPtr, protocol::ISession::TSharedPtr> m_map_stream_session;
        };

        struct Event: public IListener::Event {
            DEFINE_CREATE(Event)
            protocol::ISession::TSharedPtr session;
        };


        class CEventHandler: public threading::IWorkerHandler<Event::TSharedPtr> {
        public:
            DEFINE_IMPLEMENTATION(CEventHandler)
            CEventHandler(Sessions::TSharedPtr const &sessions);

            void    initialize  () override;
            void    finalize    () override;
            TItems  handle      (TItems const &events) override;
        private:
            Sessions::TSharedPtr    m_sessions;
        };

        typedef threading::IWorkerPool<Event::TSharedPtr> TWorkerPool;

        IListener::TSharedPtr   m_listener;
        Sessions::TSharedPtr    m_sessions;
        TWorkerPool::TSharedPtr m_worker_pool;
    };

    IStreamPool::TSharedPtr         m_listener_handler;
    threading::IThread::TSharedPtr  m_listener_thread;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
