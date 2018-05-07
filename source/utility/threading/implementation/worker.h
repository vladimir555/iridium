#ifndef HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
#define HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6


#include "utility/smart_ptr.h"

#include "async_queue.h"
#include "runnuble.h"

#include "utility/logging/logger.h"
#include "utility/threading/worker.h"
#include "utility/threading/thread.h"
#include "utility/threading/runnable.h"
#include "utility/threading/implementation/thread.h"


namespace utility {
namespace threading {
namespace implementation {


template<typename TItem>
class CWorker: public IWorker<TItem> {
public:
    DEFINE_CREATE(CWorker<TItem>)
    CWorker(
        std::string                                 const &name,
        typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler);
    CWorker(
        std::string                                 const &name,
        typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler,
        typename IAsyncQueue<TItem>::TSharedPtr     const &async_queue,
        IAsyncQueue<bool>::TSharedPtr               const &thread_working_status_queue);
    virtual ~CWorker() = default;

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void initialize() override;
    virtual void finalize() override;

    virtual size_t push(TItem  const &item)  override;
    virtual size_t push(TItems const &items) override;

private:
    class Runnuble: public CRunnuble {
    public:
        DEFINE_CREATE(Runnuble)
        Runnuble(CWorker &worker);
        virtual ~Runnuble() = default;
        void run() override;
        void initialize() override;
        void finalize() override;
    private:
        CWorker &m_worker;
    };
    typename IWorkerHandler<TItem>::TSharedPtr  m_worker_handler;
    typename IAsyncQueue<TItem>::TSharedPtr     m_async_queue;
    IRunnable::TSharedPtr   m_runnuble;
    IThread::TSharedPtr     m_thread;
};


template<typename TItem>
CWorker<TItem>::CWorker(
    std::string                                 const &name,
    typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler)
:
    m_worker_handler    (worker_handler),
    m_async_queue       (CAsyncQueue<TItem>::create()),
    m_runnuble          (Runnuble::create(*this)),
    m_thread            (CThread::create(m_runnuble, name))
{}


template<typename TItem>
CWorker<TItem>::CWorker(
    std::string                                 const &name,
    typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler,
    typename IAsyncQueue<TItem>::TSharedPtr     const &async_queue,
    IAsyncQueue<bool>::TSharedPtr               const &thread_working_status_queue)
:
    m_worker_handler    (worker_handler),
    m_async_queue       (async_queue),
    m_runnuble          (Runnuble::create(*this)),
    m_thread            (CThread::create(m_runnuble, name, thread_working_status_queue))
{}


template<typename TItem>
void CWorker<TItem>::initialize() {
    m_thread->initialize();
}


template<typename TItem>
void CWorker<TItem>::finalize() {
    m_async_queue->interrupt();
    m_thread->finalize();
}


template<typename TItem>
size_t CWorker<TItem>::push(TItem const &item) {
    return m_async_queue->push(item); // ----->
}


template<typename TItem>
size_t CWorker<TItem>::push(TItems const &items) {
    return m_async_queue->push(items); // ----->
}


template<typename TItem>
CWorker<TItem>::Runnuble::Runnuble(CWorker &worker) 
:    
    m_worker(worker)
{}


template<typename TItem>
void CWorker<TItem>::Runnuble::run() {
    try {
        while (m_is_running)
            m_worker.push(m_worker.m_worker_handler->handle(m_worker.m_async_queue->pop()));
    } catch (std::exception const &e) {
        LOGF << "worker thread '" << m_worker.m_thread->getName() << "' fatal error, stop thread: " << e.what();
        // todo: handler
        // m_worker->handleException(e);
    }
}


template<typename TItem>
void CWorker<TItem>::Runnuble::initialize() {
    m_worker.m_worker_handler->initialize();
}


template<typename TItem>
void CWorker<TItem>::Runnuble::finalize() {
    m_worker.m_worker_handler->finalize();
}


} // implementation
} // threading
} // utility


#endif // HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
