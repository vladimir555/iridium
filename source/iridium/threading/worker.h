// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
#define HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76


#include "iridium/smart_ptr.h"

#include "iridium/threading/async_queue.h"
//#include "iridium/threading/runnable.h"
#include "iridium/pattern/initializable.h"

#include <string>
#include <atomic>


namespace iridium {
namespace threading {


//template<typename TInputItem, typename TOutputItem = TInputItem>
//class IWorkerHandler: public pattern::IInitializable {
//public:
//    DEFINE_INTERFACE(IWorkerHandler);
//    typedef typename IAsyncQueuePusher<TInputItem>::TItems  TInputItems;
//    typedef typename IAsyncQueuePopper<TOutputItem>::TItems TOutputItems;
//    virtual TOutputItems handle(TInputItems const &items) = 0;
//};


template<typename TItem>
class IWorkerPusher:
    public virtual IAsyncQueuePusher<TItem>,
    public virtual pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IWorkerPusher);
    typedef typename IAsyncQueuePusher<TItem>::TItems TInputItems;
    class IHandler: public pattern::IInitializable {
    public:
        DEFINE_INTERFACE(IHandler)
        virtual void handle(TInputItems const &items) = 0;
    };
};


template<typename TItem>
class IWorkerPopper:
    public virtual IAsyncQueuePopper<TItem>,
    public virtual pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IWorkerPopper)
    typedef typename IAsyncQueuePusher<TItem>::TItems TOutputItems;
    class IHandler: public pattern::IInitializable {
    public:
        DEFINE_INTERFACE(IHandler)
        virtual TOutputItems handle() = 0;
    };
};


template<typename TInputItem, typename TOutputItem = TInputItem>
class IWorker:
    public IWorkerPusher<TInputItem>,
    public IWorkerPopper<TOutputItem>
{
public:
    DEFINE_INTERFACE(IWorker)
    typedef typename IWorkerPusher<TInputItem>::TInputItems TInputItems;
    typedef typename IWorkerPopper<TOutputItem>::TOutputItems TOutputItems;
    class IHandler: public pattern::IInitializable {
    public:
        DEFINE_INTERFACE(IHandler)
        virtual TOutputItems handle(TInputItems const &items) = 0;
    };
};


} // threading
} // iridium


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
