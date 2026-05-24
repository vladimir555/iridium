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


namespace iridium::threading::implementation {


// ----- class definidion


/// \~english @brief A thread-safe asynchronous queue implementation.
/// \~russian @brief Потокобезопасная реализация асинхронной очереди.
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
    std::list<TItem> pop(std::chrono::system_clock::duration const &timeout) override;

    void interrupt() override;
    bool empty() const override;

private:
    bool checkWaitingPredicate() const override;

    std::list<TItem> m_items;
};


// ----- class implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    LOCK_SCOPE();

    m_items.push_back(item);

    // printf("%s %p push 1, %zu\n", threading::IThread::getNameStatic().c_str(), (void*)this, m_items.size());

    return m_items.size(); // ----->
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    LOCK_SCOPE();

    m_items.insert(m_items.end(), items.begin(), items.end());

    // printf("%s %p push 2, %zu\n", threading::IThread::getNameStatic().c_str(), (void*)this, m_items.size());

    return m_items.size();
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_wait_required) {
    LOCK_SCOPE();

    if (m_items.empty() && is_wait_required)
        LOCK_SCOPE_TRY_WAIT();

    // printf("%s %p pop  1, %zu\n", threading::IThread::getNameStatic().c_str(), (void*)this, m_items.size());

    return std::move(m_items); // ----->
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::system_clock::duration const &timeout) {
    LOCK_SCOPE();

    if (m_items.empty())
        LOCK_SCOPE_TRY_WAIT(timeout);

    // printf("%s %p pop  2, %zu\n", threading::IThread::getNameStatic().c_str(), (void*)this, m_items.size());

    return std::move(m_items); // ----->
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    Synchronized::interrupt();
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    LOCK_SCOPE();

    return m_items.empty(); // ----->
}


template<typename TItem>
bool CAsyncQueue<TItem>::checkWaitingPredicate() const {
    return !m_items.empty(); // ----->
}


} // iridium::threading::implementation


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
