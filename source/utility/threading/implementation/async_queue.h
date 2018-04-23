#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include <list>

#include "utility/smart_ptr.h"
#include "utility/pattern/non_copyable.h"
#include "utility/pattern/non_movable.h"

#include "utility/threading/async_queue.h"
#include "utility/threading/condition.h"
#include "utility/threading/synchronized_scope.h"

#include "condition.h"

#include <iostream>

namespace utility {
namespace threading {
namespace implementation {


// ----- interface


// todo: waiting for processing queue on interrupting
template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    public  pattern::NonCopyable,
    public  pattern::NonMovable,

    private CCondition
{
public:
    DEFINE_CREATE(CAsyncQueue)
    ///
    CAsyncQueue();
    ///
    virtual ~CAsyncQueue() = default;
    ///
    virtual size_t push(TItem const &item) override;
    ///
    virtual size_t push(std::list<TItem> const &items) override;
    ///
    virtual std::list<TItem> pop() override;
    ///
    virtual void interrupt() override;

private:
    ///
    std::list<TItem>  m_items;
    ///
    std::atomic<bool> m_is_do_wait;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue() 
:
    m_is_do_wait(true)
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    LOCK_SCOPE_FAST

    m_items.push_back(item);
    notifyOne();

    return m_items.size(); // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    LOCK_SCOPE_FAST

    auto items_ = items;

    m_items.splice(m_items.end(), items_);
    notifyOne();

    return m_items.size(); // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop() {
    while (m_is_do_wait) {
        wait();
        LOCK_SCOPE_FAST;
        if (!m_items.empty() || !m_is_do_wait)
            return std::move(m_items); // ----->
    }
    return std::list<TItem>();
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    m_is_do_wait = false;
    notifyAll();
}


} // implementation
} // threading
} // utility


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
