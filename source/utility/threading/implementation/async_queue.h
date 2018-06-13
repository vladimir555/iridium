#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include <list>

#include "utility/smart_ptr.h"
#include "utility/pattern/non_copyable.h"
#include "utility/pattern/non_movable.h"

#include "utility/threading/async_queue.h"
#include "utility/threading/condition.h"
#include "utility/threading/synchronized.h"
#include "utility/threading/synchronized_scope.h"

#include "condition.h"


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
    private Synchronized
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
    std::list<TItem>        m_items;
    std::atomic<bool>       m_is_do_wait;
    std::atomic<bool>       m_is_empty;
    ICondition::TSharedPtr  m_condition;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
    Synchronized(CMutex::create()),
    m_is_do_wait(true),
    m_is_empty  (true),
    m_condition (CCondition::create())
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    LOCK_SCOPE_FAST

    m_items.push_back(item);
    m_is_empty = false;
    m_condition->notifyOne();

    return m_items.size(); // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    LOCK_SCOPE_FAST

    auto items_ = items;

    m_items.splice(m_items.end(), items_);
    m_is_empty = false;
    m_condition->notifyOne();

    return m_items.size(); // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop() {
    if (m_is_do_wait && m_is_empty)
        m_condition->wait();

    LOCK_SCOPE_FAST
    return std::move(m_items);
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    m_is_do_wait = false;
    m_condition->notifyAll();
}


} // implementation
} // threading
} // utility


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
