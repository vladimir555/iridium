#ifndef HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
#define HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453


#include "iridium/io/session.h"
#include "iridium/io/event_provider.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>


namespace iridium {
namespace io {
namespace implementation {


class CSessionManager: public ISessionManager {
public:
    DEFINE_IMPLEMENTATION(CSessionManager);
    CSessionManager();

    void initialize() override;
    void finalize() override;
    ///
    void manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) override;
    ///
    bool wait(std::chrono::nanoseconds const &timeout) override;

private:
    struct TContext {
        DEFINE_CREATE(TContext)
        IProtocol::TSharedPtr
            protocol;
        std::unordered_map<int, IPipe::TSharedPtr>
            map_id_pipe;
        threading::IAsyncQueue<IEvent::TSharedPtr>::TSharedPtr
            events;
    };

    class Cache: public threading::Synchronized {
    public:
        DEFINE_CREATE(Cache)
        Cache(
            IEventProvider::TSharedPtr const    &event_provider,
            std::atomic<size_t>                 &protocol_count,
            std::condition_variable             &cv);
        virtual ~Cache() = default;

        void put(IStream::TSharedPtr  const &stream, IProtocol::TSharedPtr const &protocol);
        void del(TContext::TSharedPtr const &context);
        TContext::TSharedPtr get(IEvent::TSharedPtr const &event);
    private:
        std::unordered_map<IStream::TConstSharedPtr, TContext::TSharedPtr>
            m_map_stream_context;
        std::unordered_map<TContext::TSharedPtr, IStream::TSharedPtr>
            m_map_context_stream;
        IEventProvider::TSharedPtr
            m_event_provider;
        std::atomic<size_t>
           &m_protocol_count;
        std::condition_variable
           &m_cv;
    };

    typedef threading::IWorker<IEvent::TSharedPtr> IContextWorker;

    class CEventProviderThreadHandler: public iridium::threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CEventProviderThreadHandler)
        CEventProviderThreadHandler(
            IContextWorker::TSharedPtr const &worker_pool,
            IEventProvider::TSharedPtr const &event_provider);

        void initialize() override;
        void finalize() override;

        void run(std::atomic<bool> &is_running) override;
    private:
        IEventProvider::TSharedPtr
            m_event_provider;
        IContextWorker::TSharedPtr
            m_worker_pool;
    };

    class CEventQueueLoopThreadHandler: public iridium::threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CEventQueueLoopThreadHandler)
        CEventQueueLoopThreadHandler(IContextWorker::TSharedPtr const &worker_pool);

        void initialize() override;
        void finalize() override;

        void run(std::atomic<bool> &is_running) override;
    private:
        IContextWorker::TSharedPtr
            m_worker_pool;
    };

    class CContextWorkerHandler: public IContextWorker::IHandler {
    public:
        DEFINE_IMPLEMENTATION(CContextWorkerHandler)
        CContextWorkerHandler(Cache::TSharedPtr const &cache);
        void initialize() override;
        void finalize() override;
        IContextWorker::TOutputItems handle(IContextWorker::TInputItems const &events) override;
    private:
        Cache::TSharedPtr
            m_cache;
    };

    std::mutex              m_cv_mutex;
    std::condition_variable m_cv;
    std::atomic<size_t>     m_protocol_count;

    IEventProvider::TSharedPtr
        m_event_provider;
    Cache::TSharedPtr
        m_cache;
    IContextWorker::TSharedPtr
        m_worker_pool;
    threading::IThread::TSharedPtr
        m_event_provider_thread;
    threading::IThread::TSharedPtr
        m_event_queue_loop_thread;

//        struct TEqual {
//            bool operator()(TProtocolSession const &left, TProtocolSession const &right) const;
//        };
//        struct THash {
//            size_t operator()(TProtocolSession const &right) const;
//        };
//    std::unordered_map<TProtocolSession, std::list<IStream::TSharedPtr>, TProtocolSession::THash, TProtocolSession::TEqual >
//        m_map_protocol_streams;
};



} // implementation
} // io
} // iridium


#endif // HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
