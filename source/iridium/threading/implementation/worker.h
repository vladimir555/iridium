// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
#define HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6


#include "async_queue.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/implementation/thread.h"

#include "worker_handler.h"


namespace iridium {
namespace threading {
namespace implementation {


template<typename TItem>
class CWorkerPusher: public IWorkerPusher<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorkerPusher)

    typedef typename IAsyncQueuePusher<TItem>::TItems   TItems;
    typedef typename IWorkerPusher<TItem>::IHandler     IHandler;

    CWorkerPusher(std::string const &name, typename IHandler::TSharedPtr const &handler);

    void initialize() override;
    void finalize() override;

    size_t push(TItem  const &item)  override;
    size_t push(TItems const &items) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
    IThread::TSharedPtr                     m_thread;
};


template<typename TItem>
class CWorkerPopper: public IWorkerPopper<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorkerPopper)

    typedef typename IAsyncQueuePopper<TItem>::TItems   TItems;
    typedef typename IWorkerPopper<TItem>::IHandler     IHandler;

    CWorkerPopper(std::string const &name, typename IHandler::TSharedPtr const &handler);

    void initialize() override;
    void finalize() override;

    TItems pop(bool const &is_do_wait) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
    IThread::TSharedPtr                     m_thread;
};


template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorker: public IWorker<TInputItem, TOutputItem> {
public:
    DEFINE_IMPLEMENTATION(CWorker)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems      TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems     TOutputItems;
    typedef typename IWorker<TInputItem, TOutputItem>::IHandler IHandler;

    CWorker(std::string const &name, typename IHandler::TSharedPtr const &handler);

    void initialize() override;
    void finalize() override;

    size_t push(TInputItem  const &item)  override;
    size_t push(TInputItems const &items) override;

    TOutputItems pop(bool const &is_do_wait) override;

private:
    typename IAsyncQueue<TInputItem>::TSharedPtr    m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr   m_output_queue;
    IThread::TSharedPtr                             m_thread;
};


//----- implementation


template<typename TItem>
CWorkerPusher<TItem>::CWorkerPusher(std::string const &name, typename IWorkerPusher<TItem>::IHandler::TSharedPtr const &handler)
:
    m_queue     (CAsyncQueue<TItem>::create()),
    m_thread    (CThread::create(name, CWorkerPusherRunnable<TItem>::create(handler, m_queue)))
{}


template<typename TItem>
void CWorkerPusher<TItem>::initialize() {
    m_thread->initialize();
}


template<typename TItem>
void CWorkerPusher<TItem>::finalize() {
    m_queue->interrupt();
    m_thread->finalize();
}


template<typename TItem>
size_t CWorkerPusher<TItem>::push(TItem const &item) {
    return m_queue->push(item);
}


template<typename TItem>
size_t CWorkerPusher<TItem>::push(TItems const &items) {
    return m_queue->push(items);
}


template<typename TItem>
CWorkerPopper<TItem>::CWorkerPopper(std::string const &name, typename IWorkerPopper<TItem>::IHandler::TSharedPtr const &handler)
:
    m_queue     (CAsyncQueue<TItem>::create()),
    m_thread    (CThread::create(name, CWorkerPopperRunnable<TItem>::create(handler, m_queue)))
{}


template<typename TItem>
void CWorkerPopper<TItem>::initialize() {
    m_thread->initialize();
}


template<typename TItem>
void CWorkerPopper<TItem>::finalize() {
    m_queue->interrupt();
    m_thread->finalize();
}


template<typename TItem>
typename CWorkerPopper<TItem>::TItems CWorkerPopper<TItem>::pop(bool const &is_do_wait) {
    return m_queue->pop(is_do_wait);
}


template<typename TInputItem, typename TOutputItem>
CWorker<TInputItem, TOutputItem>::CWorker(
    std::string const &name,
    typename IWorker<TInputItem, TOutputItem>::IHandler::TSharedPtr const &handler)
:
    m_input_queue   (CAsyncQueue<TInputItem>::create()),
    m_output_queue  (CAsyncQueue<TOutputItem>::create()),
    m_thread        (CThread::create(name, CWorkerRunnable<TInputItem, TOutputItem>::create(handler, m_input_queue, m_output_queue)))
{}


template<typename TInputItem, typename TOutputItem>
void CWorker<TInputItem, TOutputItem>::initialize() {
    m_thread->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorker<TInputItem, TOutputItem>::finalize() {
    m_input_queue->interrupt();
    m_output_queue->interrupt();
    m_thread->finalize();
}


template<typename TInputItem, typename TOutputItem>
size_t CWorker<TInputItem, TOutputItem>::push(TInputItem const &item) {
    return m_input_queue->push(item);
}


template<typename TInputItem, typename TOutputItem>
size_t CWorker<TInputItem, TOutputItem>::push(TInputItems const &items) {
    return m_input_queue->push(items);
}


template<typename TInputItem, typename TOutputItem>
typename CWorker<TInputItem, TOutputItem>::TOutputItems CWorker<TInputItem, TOutputItem>::pop(bool const &is_do_wait) {
    return m_output_queue->pop(is_do_wait);
}


} // implementation
} // threading
} // iridium


#endif // HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
