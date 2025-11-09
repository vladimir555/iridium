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


// ----- class definidion


template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    private Synchronized<std::mutex>,
    private pattern::NonCopyable
{
public:
    DEFINE_IMPLEMENTATION(CAsyncQueue)
    CAsyncQueue();
    size_t push(TItem const &item) override;
    size_t push(std::list<TItem> const &items) override;

    std::list<TItem> pop(bool const &is_wait_required = true) override;
    std::list<TItem> pop(std::chrono::nanoseconds const &timeout) override;

    void interrupt() override;

    bool empty() const override;

private:
    std::list<TItem>    m_items;
    std::atomic<bool>   m_is_empty;
};


// ----- class implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
    m_is_empty  (true)
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    LOCK_SCOPE();
    m_items.push_back(item);
    m_is_empty = m_items.empty();
    auto size  = m_items.size();

    return size; // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    LOCK_SCOPE();

    m_items.insert(m_items.end(), items.begin(), items.end());
    m_is_empty = m_items.empty();

    return m_items.size();
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_wait_required) {
    LOCK_SCOPE();
    if (m_is_empty && is_wait_required)
        LOCK_SCOPE_TRY_WAIT();

    m_is_empty = true;
    return std::move(m_items); // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    LOCK_SCOPE();

    if (m_is_empty)
        LOCK_SCOPE_TRY_WAIT(timeout);

    m_is_empty = true;

    return std::move(m_items); // ----->
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    Synchronized::interrupt();
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
