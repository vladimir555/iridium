// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
#define HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47


#include "iridium/threading/thread.h"
#include "iridium/threading/worker.h"
#include "iridium/convertion/convert.h"

#include "worker.h"
#include "async_queue.h"

#include <list>


using iridium::convertion::convert;
using std::string;


namespace iridium {
namespace threading {
namespace implementation {


// ----- interface


class WorkerPoolBase {
public:
    virtual ~WorkerPoolBase() = default;

protected:
    WorkerPoolBase(std::string const &name);
    void waitForMultipleStatus(bool const &status);

    std::list<IThread::TSharedPtr>  m_threads;

private:
    std::string m_name;
};


template<typename TItem>
class CWorkerPoolPusher: public IWorkerPusher<TItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPoolPusher)

    typedef typename IWorkerPusher<TItem>::TItems                           TItems;
    typedef std::list<typename IWorkerPusher<TItem>::IHandler::TSharedPtr>  THandlers;

    CWorkerPoolPusher(std::string const &name, THandlers const &handlers);

    void initialize() override;
    void finalize() override;

    size_t push(TItem  const &item)  override;
    size_t push(TItems const &items) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
};


template<typename TItem>
class CWorkerPoolPopper: public IWorkerPopper<TItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPoolPopper)

    typedef typename IAsyncQueuePopper<TItem>::TItems                       TItems;
    typedef std::list<typename IWorkerPopper<TItem>::IHandler::TSharedPtr>  THandlers;

    CWorkerPoolPopper(std::string const &name, THandlers const &handlers);

    void initialize() override;
    void finalize() override;

    TItems pop(bool const &is_do_wait) override;
    TItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
};


template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorkerPool: public IWorker<TInputItem, TOutputItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPool)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems                              TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems                             TOutputItems;
    typedef std::list<typename IWorker<TInputItem, TOutputItem>::IHandler::TSharedPtr>  THandlers;

    CWorkerPool(std::string const &name, THandlers const &handlers);

    void initialize() override;
    void finalize() override;

    size_t push(TInputItem  const &item)  override;
    size_t push(TInputItems const &items) override;

    TOutputItems pop(bool const &is_do_wait) override;
    TOutputItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    typename IAsyncQueue<TInputItem>::TSharedPtr    m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr   m_output_queue;
};


//----- implementation


template<typename TItem>
CWorkerPoolPusher<TItem>::CWorkerPoolPusher(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_queue         (CAsyncQueue<TItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(
            CThread::create(name + "[" + convert<string>(i++) + "]",
            CWorkerPusherRunnable<TItem>::create(handler, m_queue)));
}


template<typename TItem>
void CWorkerPoolPusher<TItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TItem>
void CWorkerPoolPusher<TItem>::finalize() {
    m_queue->interrupt();
    for (auto const &thread: m_threads)
        thread->finalize();
}


template<typename TItem>
size_t CWorkerPoolPusher<TItem>::push(TItem const &item) {
    return m_queue->push(item);
}


template<typename TItem>
size_t CWorkerPoolPusher<TItem>::push(TItems const &items) {
    return m_queue->push(items);
}


template<typename TItem>
CWorkerPoolPopper<TItem>::CWorkerPoolPopper(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_queue         (CAsyncQueue<TItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(
            CThread::create(name + "[" + convert<string>(i++) + "]",
            CWorkerPopperRunnable<TItem>::create(handler, m_queue)));
}


template<typename TItem>
void CWorkerPoolPopper<TItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TItem>
void CWorkerPoolPopper<TItem>::finalize() {
    for (auto const &thread: m_threads)
        thread->finalize();
    m_queue->interrupt();
}


template<typename TItem>
typename CWorkerPoolPopper<TItem>::TItems CWorkerPoolPopper<TItem>::pop(bool const &is_do_wait) {
    return m_queue->pop(is_do_wait); // ----->
}


template<typename TItem>
typename CWorkerPoolPopper<TItem>::TItems CWorkerPoolPopper<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    return m_queue->pop(timeout); // ----->
}


template<typename TInputItem, typename TOutputItem>
CWorkerPool<TInputItem, TOutputItem>::CWorkerPool(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_input_queue   (CAsyncQueue<TInputItem>::create()),
    m_output_queue  (CAsyncQueue<TOutputItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(CThread::create(
            name + "[" + convert<string>(i++) + "]",
            CWorkerRunnable<TInputItem, TOutputItem>::create(
                handler, m_input_queue, m_output_queue)));
}


template<typename TInputItem, typename TOutputItem>
void CWorkerPool<TInputItem, TOutputItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorkerPool<TInputItem, TOutputItem>::finalize() {
    m_input_queue->interrupt();
    m_output_queue->interrupt();
    for (auto const &thread: m_threads)
        thread->finalize();
    m_output_queue->interrupt();
    m_input_queue->interrupt();
}


template<typename TInputItem, typename TOutputItem>
size_t CWorkerPool<TInputItem, TOutputItem>::push(TInputItem const &item) {
    return m_input_queue->push(item);
}


template<typename TInputItem, typename TOutputItem>
size_t CWorkerPool<TInputItem, TOutputItem>::push(TInputItems const &items) {
    return m_input_queue->push(items);
}


template<typename TInputItem, typename TOutputItem>
typename CWorkerPool<TInputItem, TOutputItem>::TOutputItems CWorkerPool<TInputItem, TOutputItem>::pop(bool const &is_do_wait) {
    return m_output_queue->pop(is_do_wait);
}


template<typename TInputItem, typename TOutputItem>
typename CWorkerPool<TInputItem, TOutputItem>::TOutputItems CWorkerPool<TInputItem, TOutputItem>::pop(std::chrono::nanoseconds const &timeout) {
    return m_output_queue->pop(timeout);
}


//    template<typename TWorkerHandler, typename ... TArgs>
//    static TSharedPtr create(std::string const &name, size_t const &count, TArgs ... args) {
//        typename IWorkerPool<TItem>::TWorkerHandlers handlers;
//        for (size_t i = 0; i < count; i++) {
//            auto handler = TWorkerHandler::create(args ...);
//            handlers.push_back(handler);
//        }
//        return create(name, handlers);
//    }


} // implementation
} // threading
} // iridium


#endif // HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
