#ifndef HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
#define HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47


#include "utility/threading/worker_pool.h"
#include "utility/threading/thread.h"
#include "utility/threading/worker.h"
#include "utility/convertion/convert.h"

#include "worker.h"
#include "async_queue.h"

#include <list>


namespace utility {
namespace threading {
namespace implementation {


// ----- interface


template<typename TItem>
class CWorkerPool: public IWorkerPool<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorkerPool<TItem>)

    explicit CWorkerPool(
        typename IWorkerPool<TItem>::TWorkerHandlers const &handlers, 
        std::string const &name);

    typedef typename IWorkerPool<TItem>::TItems TItems;

    void initialize() override;
    void finalize() override;

    size_t push(TItem  const &item)  override;
    size_t push(TItems const &items) override;

private:
    void waitForMultipleStatus(bool const &status);

    std::string                                     m_name;
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    std::list<typename IWorker<TItem>::TSharedPtr>  m_workers;
    IAsyncQueue<bool>::TSharedPtr                   m_thread_working_status_queue;
};


// ----- implementation


template<typename TItem>
CWorkerPool<TItem>::CWorkerPool(
    typename IWorkerPool<TItem>::TWorkerHandlers const &handlers, 
    std::string const &name)
:
    m_name(name)
{
    m_queue = CAsyncQueue<TItem>::create();

    m_thread_working_status_queue = CAsyncQueue<bool>::create();
    auto i = 0;
    for (auto const &handler: handlers) {
        m_workers.push_back(
            CWorker<TItem>::create(
                name + "[" + convertion::convert<std::string>(i) + "]", 
                handler, 
                m_queue, 
                m_thread_working_status_queue
            ) 
        );
        i++;
    }
}


template<typename TItem>
void CWorkerPool<TItem>::initialize() {
    static auto const THREAD_START_TIMEOUT_MS = 10000;

    for (auto &worker: m_workers)
        worker->initialize();

    waitForMultipleStatus(true);
}


template<typename TItem>
void CWorkerPool<TItem>::finalize() {
    for (auto &worker: m_workers)
        worker->finalize();

    waitForMultipleStatus(false);
}


template<typename TItem>
size_t CWorkerPool<TItem>::push(TItem const &item) {
    return m_queue->push(item); // ----->
}


template<typename TItem>
size_t CWorkerPool<TItem>::push(TItems const &items) {
    return m_queue->push(items); // ----->
}


template<typename TItem>
void CWorkerPool<TItem>::waitForMultipleStatus(bool const &status) {
    // todo: wait timeout
    try {
        size_t count = 0;
        while (count < m_workers.size()) {
            for (auto const &s : m_thread_working_status_queue->pop())
                if (status == s)
                    count++;
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("worker pool '" + m_name + "' start error: " + e.what()); // ----->
    }
}


} // implementation
} // threading
} // utility


#endif // HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
