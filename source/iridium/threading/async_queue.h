// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
#define HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE


#include <list>
#include <chrono>
#include "iridium/smart_ptr.h"


namespace iridium::threading {


// todo: must be SyncQueue
// todo: rename to ThreadSafeQueuePusher
// todo: virtual public ISynchronized
template<typename TItem>
class IAsyncQueuePusher {
public:
    DEFINE_INTERFACE(IAsyncQueuePusher)

    typedef std::list<TItem> TItems;

    virtual size_t push(TItem  const &item)  = 0;
    virtual size_t push(TItems const &items) = 0;
};


// todo: virtual public ISynchronized
template<typename TItem>
class IAsyncQueuePopper {
public:
    DEFINE_INTERFACE(IAsyncQueuePopper)

    typedef std::list<TItem> TItems;

    virtual TItems pop(bool const &is_wait_required = true) = 0;
    virtual TItems pop(std::chrono::nanoseconds const &timeout) = 0;
};


// todo: rename to ThreadSafeQueue
template<typename TItem>
class IAsyncQueue:
    public IAsyncQueuePusher<TItem>,
    public IAsyncQueuePopper<TItem>
{
public:
    DEFINE_INTERFACE(IAsyncQueue)

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual void interrupt() = 0;
    virtual bool empty() const = 0;
};


} // namespace iridium::threading


#endif // HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
