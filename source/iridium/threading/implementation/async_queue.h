// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include "iridium/pattern/non_copyable.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/synchronized.h"
#include "iridium/logging/logger.h"

#include <atomic>


namespace iridium {
namespace threading {
namespace implementation {


// ----- interface


template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    private Synchronized<std::mutex, true>,
    private pattern::NonCopyable
{
public:
    DEFINE_IMPLEMENTATION(CAsyncQueue)
    ///
    CAsyncQueue();
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
    std::list<TItem>    m_items;
    std::atomic<bool>   m_is_waiting;
    std::atomic<bool>   m_is_empty;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
    m_is_waiting(true),
    m_is_empty  (true)
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
//    LOCK_SCOPE();
//
//    m_items.push_back(item);
//    m_is_empty = m_items.empty();
//
//    LOGT << "M: " << m_mutex_name << " push items, size: " << m_items.size() << ", m_is_empty: " << m_is_empty;
//
//    return m_items.size();

    {
        std::unique_lock<std::mutex> l(m_mutex);

        m_items.push_back(item);
        m_is_empty = m_items.empty();
        auto size  = m_items.size();

        l.unlock();
        m_cv.notify_one();

        return size; // ----->
    }
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
//    LOCK_SCOPE();
//
//    m_items.insert(m_items.end(), items.begin(), items.end());
//    m_is_empty = m_items.empty();
//
//    LOGT << "M: " << m_mutex_name << " push item, size: " << m_items.size() << ", m_is_empty: " << m_is_empty;
//
//    return m_items.size();
    
    
    {
        std::unique_lock<std::mutex> l(m_mutex);

        m_items.insert(m_items.end(), items.begin(), items.end());
        m_is_empty  = m_items.empty();
        auto size   = m_items.size();

        l.unlock();
        m_cv.notify_one();

        return size; // ----->
    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_waiting) {
//    if (m_is_waiting && m_is_empty && is_waiting) {
//        LOGT << "M: " << m_mutex_name << " pop waiting, m_is_waiting: " << m_is_waiting
//             << ", m_is_empty: " << m_is_empty
//             << ", is_waiting: " << is_waiting;
//
//        LOCK_SCOPE_WAIT() {
//            m_is_empty = true;
//            LOGT << "pop waiting, size: " << m_items.size();
//            return std::move(m_items); // ----->
//        }
//    }
//
//    LOCK_SCOPE();
//    m_is_empty = true;
//    return std::move(m_items); // ----->
    
    {
        std::unique_lock<std::mutex> l(m_mutex);
        while (m_is_waiting && m_is_empty && is_waiting)
            m_cv.wait(l);

        m_is_empty = true;
        return std::move(m_items); // ----->
    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::nanoseconds const &timeout) {
//    if (m_is_waiting && m_is_empty) {
//        LOGT << "M: " << m_mutex_name << " pop timed, m_is_waiting: " << m_is_waiting
//             << ", m_is_empty: " << m_is_empty;
//
//        LOCK_SCOPE_WAIT(timeout) {
//            m_is_empty = true;
//            LOGT << "pop timed, size: " << m_items.size();
//            return std::move(m_items); // ----->
//        }
//    }
//
//    LOCK_SCOPE();
//    m_is_empty = true;
//    return std::move(m_items);
    

    {
        std::unique_lock<std::mutex> l(m_mutex);

        if (m_is_waiting && m_is_empty)
            m_cv.wait_for(l, timeout);

        m_is_empty = true;

        return std::move(m_items); // ----->
    }
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
//    m_is_waiting = false;
//    Synchronized::interrupt();

    {
        m_is_waiting = false;
        m_cv.notify_all();
    }
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
