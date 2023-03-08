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


#include <iostream>
namespace iridium {
namespace threading {
namespace implementation {


// ----- interface


// todo: waiting for processing queue on interrupting
template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    public  pattern::NonCopyable,
//    public  pattern::NonMovable,
    public  CCondition,
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
    std::list<TItem> pop(std::chrono::nanoseconds const &timeout) override;
    ///
    void interrupt() override;
    ///
    bool empty() const override;
private:
    std::list<TItem>        m_items;
    std::atomic<bool>       m_is_waiting;
    std::atomic<bool>       m_is_empty;
//    ICondition::TSharedPtr  m_condition;
    std::mutex              m_mutex;
    std::condition_variable m_condition;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
//    Synchronized(),
    m_is_waiting(true),
    m_is_empty  (true)
//    m_condition (CCondition::create())
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
//    LOCK_SCOPE_FAST

//    m_items.push_back(item);
//    m_is_empty = m_items.empty();
//    m_condition->notifyOne();

//    return m_items.size(); // ----->

    std::unique_lock<std::mutex> l(m_mutex);

    m_items.push_back(item);
    m_is_empty = m_items.empty();
    auto size  = m_items.size();

    l.unlock();
    m_condition.notify_one();

    return size; // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
//    LOCK_SCOPE_FAST

//    m_items.insert(m_items.end(), items.begin(), items.end());
//    m_is_empty = m_items.empty();
//    m_condition->notifyOne();

//    return m_items.size(); // ----->

    std::unique_lock<std::mutex> l(m_mutex);

    m_items.insert(m_items.end(), items.begin(), items.end());
    m_is_empty  = m_items.empty();
    auto size   = m_items.size();

    l.unlock();
    m_condition.notify_one();

    return size; // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_waiting) {
//    if (is_waiting && m_is_waiting && m_is_empty)
//        m_condition->wait();

//    LOCK_SCOPE_FAST
//    auto result = std::move(m_items);
//    m_is_empty = true;
//    return result;

    std::unique_lock<std::mutex> l(m_mutex);
    while (m_is_waiting && m_is_empty && is_waiting)
        m_condition.wait(l);

    m_is_empty = true;
    return std::move(m_items); // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::nanoseconds const &timeout) {
//    if (m_is_waiting && m_is_empty)
//        m_condition->wait(timeout);

//    LOCK_SCOPE_FAST
//    auto result = std::move(m_items);
//    m_is_empty = true;
//    return result;

    std::unique_lock<std::mutex> l(m_mutex);

    if (m_is_waiting && m_is_empty)
        m_condition.wait_for(l, timeout);

//    auto now = std::chrono::system_clock::now;
//    auto time_end = now() + timeout;
//    while (m_is_waiting && m_is_empty && now() < time_end)
//        m_condition.wait_until(l, time_end);

    m_is_empty = true;
    return std::move(m_items); // ----->
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
//    m_is_waiting = false;
//    m_condition->notifyAll();

    m_is_waiting = false;
    m_condition.notify_all();
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
