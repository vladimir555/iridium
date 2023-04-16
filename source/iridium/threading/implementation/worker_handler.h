// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33
#define HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33


#include "iridium/threading/runnable.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/async_queue.h"


namespace iridium {
namespace threading {
namespace implementation {


template<typename TItem>
class CWorkerPusherRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerPusherRunnable)

    typedef typename IAsyncQueuePusher<TItem>::TItems   TItems;
    typedef typename IWorkerPusher<TItem>::IHandler     IHandler;

    CWorkerPusherRunnable(
        typename IHandler::TSharedPtr const &handler,
        typename IAsyncQueue<TItem>::TSharedPtr const &queue);

    void run(std::atomic<bool> &is_running) override;
    void initialize() override;
    void finalize() override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    typename IHandler::TSharedPtr                   m_handler;
};


template<typename TItem>
class CWorkerPopperRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerPopperRunnable)

    typedef typename IAsyncQueuePopper<TItem>::TItems   TItems;
    typedef typename IWorkerPopper<TItem>::IHandler     IHandler;

    CWorkerPopperRunnable(
        typename IHandler::TSharedPtr           const &handler,
        typename IAsyncQueue<TItem>::TSharedPtr const &queue);

    void run(std::atomic<bool> &is_running) override;
    void initialize() override;
    void finalize() override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    typename IHandler::TSharedPtr                   m_handler;
};


template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorkerRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerRunnable)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems      TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems     TOutputItems;
    typedef typename IWorker<TInputItem, TOutputItem>::IHandler IHandler;

    CWorkerRunnable(
        typename IHandler::TSharedPtr                   const &handler,
        typename IAsyncQueue<TInputItem>::TSharedPtr    const &m_input_queue,
        typename IAsyncQueue<TOutputItem>::TSharedPtr   const &m_output_queue);

    void run(std::atomic<bool> &is_running) override;
    void initialize() override;
    void finalize() override;

private:
    typename IAsyncQueue<TInputItem>::TSharedPtr    m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr   m_output_queue;
    typename IHandler::TSharedPtr                   m_handler;
};


//----- implementation


template<typename TItem>
CWorkerPusherRunnable<TItem>::CWorkerPusherRunnable(
    typename IHandler::TSharedPtr const &handler,
    typename IAsyncQueue<TItem>::TSharedPtr const &queue)
:
    m_queue     (queue),
    m_handler   (handler)
{}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto items = m_queue->pop();
        if (!items.empty())
            m_handler->handle(items);
    }
}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::initialize() {
    m_handler->initialize();
}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::finalize() {
    m_handler->finalize();
}


template<typename TItem>
CWorkerPopperRunnable<TItem>::CWorkerPopperRunnable(
    typename IHandler::TSharedPtr const &handler,
    typename IAsyncQueue<TItem>::TSharedPtr const &queue)
:
    m_queue     (queue),
    m_handler   (handler)
{}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::run(std::atomic<bool> &is_running) {
    while (is_running)
        m_queue->push(m_handler->handle());
}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::initialize() {
    m_handler->initialize();
}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::finalize() {
    m_handler->finalize();
}


template<typename TInputItem, typename TOutputItem>
CWorkerRunnable<TInputItem, TOutputItem>::CWorkerRunnable(
    typename IHandler::TSharedPtr                   const &handler,
    typename IAsyncQueue<TInputItem>::TSharedPtr    const &input_queue,
    typename IAsyncQueue<TOutputItem>::TSharedPtr   const &output_queue)
:
    m_input_queue   (input_queue),
    m_output_queue  (output_queue),
    m_handler       (handler)
{}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::run(std::atomic<bool> &is_running) {
    while (is_running)
        m_output_queue->push(m_handler->handle(m_input_queue->pop()));
}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::initialize() {
    m_handler->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::finalize() {
    m_handler->finalize();
}


} // implementation
} // threading
} // iridium


#endif // HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33
