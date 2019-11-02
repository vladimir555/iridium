// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
#define HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6


#include "iridium/smart_ptr.h"

#include "async_queue.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/implementation/thread.h"


namespace iridium {
namespace threading {
namespace implementation {


class CWorkerHandler: public IWorkerHandler<IJob::TSharedPtr> {
public:
    DEFINE_IMPLEMENTATION(CWorkerHandler)
    CWorkerHandler() = default;

    void initialize() override;
    void finalize() override;
    TItems handle(TItems const &items) override;
};


template<typename TItem = IJob::TSharedPtr>
class CWorker: public IWorker<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorker<TItem>)
    CWorker(
        std::string                                 const &name,
        typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler = CWorkerHandler::create());
    CWorker(
        std::string                                 const &name,
        typename IWorkerHandler<TItem>::TSharedPtr  const &worker_handler,
        typename IAsyncQueue<TItem>::TSharedPtr     const &async_queue,
        IAsyncQueue<bool>::TSharedPtr               const &thread_working_status_queue);

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void initialize() override;
    virtual void finalize() override;

    virtual size_t push(TItem  const &item)  override;
    virtual size_t push(TItems const &items) override;

private:
    class Runnuble: public IRunnable {
    public:
        DEFINE_IMPLEMENTATION(Runnuble)

        Runnuble(CWorker &worker);
        void run(std::atomic<bool> &is_running) override;
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
void CWorker<TItem>::Runnuble::run(std::atomic<bool> &is_running) {
    try {
        while (is_running)
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
} // iridium


#endif // HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
