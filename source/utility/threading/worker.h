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


class IJob {
public:
    DEFINE_INTERFACE(IJob)
    virtual bool do_() = 0;
};


template<typename TItem_ = IJob::TSharedPtr>
class IWorkerHandler {
public:
    DEFINE_SMART_PTR(IWorkerHandler<TItem_>)
    virtual ~IWorkerHandler() = default;

    typedef TItem_ TItem;
    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual TItems handle(TItems const &items) = 0;
};


template<typename TItem = IJob::TSharedPtr>
class IWorker:
    public IAsyncQueuePusher<TItem>,
    public pattern::IInitializable
{
public:
    DEFINE_SMART_PTR(IWorker<TItem>)
    virtual ~IWorker() = default;

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;
};


} // threading
} // utility


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
