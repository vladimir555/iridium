#ifndef HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
#define HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47


#include "utility/threading/worker_pool.h"
#include "utility/threading/thread.h"
#include "utility/threading/worker.h"
#include "utility/threading/implementation/worker.h"

#include "utility/convertion/convert.h"

#include <list>


namespace utility {
namespace threading {
namespace implementation {


// ----- interface


template<typename TItem>
class CWorkerPool: public IWorkerPool<TItem> {
public:
    DEFINE_CREATE(CWorkerPool<TItem>)
    explicit CWorkerPool(typename IWorkerPool<TItem>::TWorkerHandlers const &handlers, std::string const &name);
    virtual ~CWorkerPool() = default;

    typedef typename IWorkerPool<TItem>::TItems TItems;

    void initialize() override;
    void finalize() override;

    size_t push(TItem  const &item)  override;
    size_t push(TItems const &items) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    std::list<typename IWorker<TItem>::TSharedPtr>  m_workers;
};


// ----- implementation


template<typename TItem>
CWorkerPool<TItem>::CWorkerPool(typename IWorkerPool<TItem>::TWorkerHandlers const &handlers, std::string const &name) {
    m_queue = CAsyncQueue<TItem>::create();

    auto i = 0;
    for (auto const &handler: handlers) {
        m_workers.push_back(
            CWorker<TItem>::create(
                name + "[" + convertion::convert<std::string>(i) + "]", handler, m_queue) );
        i++;
    }
}


template<typename TItem>
void CWorkerPool<TItem>::initialize() {
    for (auto &worker: m_workers)
        worker->initialize();
}


template<typename TItem>
void CWorkerPool<TItem>::finalize() {
    for (auto &worker: m_workers)
        worker->finalize();
}


template<typename TItem>
size_t CWorkerPool<TItem>::push(TItem const &item) {
    return m_queue->push(item); // ----->
}


template<typename TItem>
size_t CWorkerPool<TItem>::push(TItems const &items) {
    return m_queue->push(items); // ----->
}


} // implementation
} // threading
} // utility


#endif // HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
