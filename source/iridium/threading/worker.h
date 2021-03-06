// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
#define HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76


#include "iridium/smart_ptr.h"

#include "iridium/threading/async_queue.h"
#include "iridium/threading/runnable.h"
#include "iridium/pattern/initializable.h"

#include <string>
#include <atomic>


namespace iridium {
namespace threading {


class IJob {
public:
    DEFINE_INTERFACE(IJob)
    virtual bool execute() = 0;
};


template<typename TItem_ = IJob::TSharedPtr>
class IWorkerHandler: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IWorkerHandler<TItem_>)

    typedef TItem_ TItem;
    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void handle(TItems const &items) = 0;
};


template<typename TItem = IJob::TSharedPtr>
class IWorker:
    public IAsyncQueuePusher<TItem>,
    public pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IWorker<TItem>)

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;
};


} // threading
} // iridium


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
