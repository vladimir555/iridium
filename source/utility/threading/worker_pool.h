#ifndef HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F
#define HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/threading/worker.h"

#include <list>


namespace utility {
namespace threading {


template<typename TItem = IJob::TSharedPtr>
class IWorkerPool:
    public pattern::IInitializable,
    public IAsyncQueuePusher<TItem>
{
//    static_assert(std::is_base_of< IWorkerHandler<typename TWorkerHandler::TItem>, TWorkerHandler >::value,
//        "TWorker must be inherit from IWorkerHandler");
public:
    typedef           IWorkerHandler<TItem> TWorkerHandler;
    typedef std::list<typename TWorkerHandler::TSharedPtr> TWorkerHandlers;
    DEFINE_INTERFACE(IWorkerPool<TItem>)
};


} // threading
} // utility


#endif // HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F
