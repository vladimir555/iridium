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


// todo: interfaces: base handlers, worker from IAsyncqueue, some handlers extends base handlers

///
template<typename TItem>
class IWorker: public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(IWorker<TItem>)
    ///
    virtual ~IWorker() = default;
    ///
    typedef std::list<TItem> TItems;
    ///
    virtual size_t push(TItem const &item) = 0;
    ///
    virtual size_t push(TItems const &items) = 0;
    /// handler calling until throw exception or return true
    virtual bool handleStart() = 0;
    ///
    virtual void handleFinish() = 0;
protected:
    ///
    virtual void handleItems(TItems const &itemS) = 0;
};


} // threading
} // utility


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
