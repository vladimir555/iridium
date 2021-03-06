// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include <list>

#include "iridium/smart_ptr.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"

#include "iridium/threading/async_queue.h"
#include "iridium/threading/condition.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/synchronized_scope.h"

#include "condition.h"


//#include <iostream>
namespace iridium {
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
    ~CAsyncQueue() = default;
    ///
    size_t push(TItem const &item) override;
    ///
    size_t push(std::list<TItem> const &items) override;
    ///
    std::list<TItem> pop(bool const &is_waiting = true) override;
    ///
    void interrupt() override;
    ///
    bool empty() override;
private:
    std::list<TItem>        m_items;
    std::atomic<bool>       m_is_waiting;
    std::atomic<bool>       m_is_empty;
    ICondition::TSharedPtr  m_condition;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
    Synchronized(CMutex::create()),
    m_is_waiting(true),
    m_is_empty  (true),
    m_condition (CCondition::create())
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    LOCK_SCOPE_FAST

    m_items.push_back(item);
    m_is_empty = m_items.empty();
    m_condition->notifyOne();

    return m_items.size(); // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    if (items.size() > 0) {
        LOCK_SCOPE_FAST

        auto items_ = items;

        m_items.splice(m_items.end(), items_);
        m_is_empty = m_items.empty();
        m_condition->notifyOne();

        return m_items.size(); // ----->
    } else
        return 0; // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_waiting) {
//    std::cout << "pop " << is_waiting << " " << m_is_waiting << " " << m_is_empty << std::endl;
    if (is_waiting && m_is_waiting && m_is_empty)
        m_condition->wait();

    LOCK_SCOPE_FAST
    m_is_empty = true;
    return std::move(m_items);
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    m_is_waiting = false;
    m_condition->notifyAll();
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
