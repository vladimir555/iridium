#ifndef HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
#define HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE


#include <list>
#include "utility/smart_ptr.h"


namespace utility {
namespace threading {


template<typename TItem>
class IAsyncQueuePusher {
public:
    DEFINE_SMART_PTR(IAsyncQueuePusher)
    virtual ~IAsyncQueuePusher() = default;

    typedef std::list<TItem> TItems;

    virtual size_t push(TItem  const &item) = 0;
    virtual size_t push(TItems const &items) = 0;
};


template<typename TItem>
class IAsyncQueue: public IAsyncQueuePusher<TItem> {
public:
    DEFINE_SMART_PTR(IAsyncQueue)
    virtual ~IAsyncQueue() = default;

    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    virtual TItems  pop() = 0;
    virtual void    interrupt() = 0;
};


} // threading
} // utility


#endif // HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
