#ifndef HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
#define HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76


#include "utility/smart_ptr.h"

#include "utility/threading/async_queue.h"
#include "utility/threading/runnable.h"
#include "utility/pattern/initializable.h"

#include <string>
#include <atomic>


namespace utility {
namespace threading {


template<typename TItem_>
class IWorkerHandler {
public:
    DEFINE_SMART_PTR(IWorkerHandler<TItem_>)
    virtual ~IWorkerHandler() = default;

    typedef TItem_ TItem;
    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void handleStart()  = 0;
    virtual void handleStop() = 0;
    virtual void handleItems(TItems const &items) = 0;
};


template<typename TItem>
class IWorker:
    public IAsyncQueuePusher<TItem>,
    public pattern::IInitializable,
    protected IWorkerHandler<TItem>
{
public:
    DEFINE_SMART_PTR(IWorker<TItem>)
    virtual ~IWorker() = default;

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;
};


} // threading
} // utility


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
