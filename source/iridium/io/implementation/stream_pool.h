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


class CStreamPool:
    public IStreamPool,
    public threading::Synchronized,
    public std::enable_shared_from_this<CStreamPool>
{
public:
    DEFINE_IMPLEMENTATION(CStreamPool)
    CStreamPool();

    typedef IListener::Event    Event;
    typedef protocol::ISession  ISession;

    void add(ISession::TSharedPtr const &session) override;
    void del(ISession::TSharedPtr const &session) override;

    void initialize()   override;
    void finalize()     override;

private:
    typedef threading::IAsyncQueue<Event::TSharedPtr> TIEventQueue;

    friend class CListenerHandler;
    class CListenerHandler: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CListenerHandler)
        CListenerHandler(CStreamPool::TWeakPtr const &stream_pool);

        void initialize()   override;
        void finalize()     override;

        void run(std::atomic<bool> &is_running) override;

    private:
        CStreamPool::TWeakPtr       m_stream_pool;
    };

    struct TContext {
        DEFINE_CREATE(TContext)
        ISession::TSharedPtr        m_session;
        TIEventQueue::TSharedPtr    m_events;
    };

    std::map<ISession::TSharedPtr, TIEventQueue::TSharedPtr>    m_map_session_events;
    std::map<IStream::TSharedPtr, ISession::TSharedPtr>         m_map_stream_session;

    TContext::TSharedPtr getContext(ISession::TSharedPtr const &session, Event::TSharedPtr const &event);

    friend class CContextHandler;
    class CContextHandler: public threading::IWorkerHandler<TContext::TSharedPtr> {
    public:
        DEFINE_IMPLEMENTATION(CContextHandler)
        CContextHandler(CStreamPool::TWeakPtr const &stream_pool);

        void initialize  () override;
        void finalize    () override;
        void handle      (TItems const &events) override;
    private:
        CStreamPool::TWeakPtr m_stream_pool;
    };


    bool                            m_is_initialized;
    IListener::TSharedPtr           m_listener;
    threading::IThread::TSharedPtr  m_listener_thread;
    threading::IWorkerPool<TContext::TSharedPtr>::TSharedPtr m_context_handler_pool;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_POOL_F0C3808C_0370_48C3_A5D8_8382381CC8FA
