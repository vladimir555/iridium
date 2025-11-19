// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
#define HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE


#include <list>
#include <chrono>
#include "iridium/smart_ptr.h"


namespace iridium::threading {


// todo: must be SyncQueue
// todo: rename to ThreadSafeQueuePusher
// todo: virtual public ISynchronized
/// \~english @brief An interface for pushing items to an asynchronous queue.
/// \~russian @brief Интерфейс для добавления элементов в асинхронную очередь.
template<typename TItem>
class IAsyncQueuePusher {
public:
    DEFINE_INTERFACE(IAsyncQueuePusher)

    /// \~english @brief A list of items.
    /// \~russian @brief Список элементов.
    typedef std::list<TItem> TItems;

    /// \~english @brief Pushes a single item to the queue.
    /// \~russian @brief Добавляет один элемент в очередь.
    virtual size_t push(TItem  const &item)  = 0;
    /// \~english @brief Pushes a list of items to the queue.
    /// \~russian @brief Добавляет список элементов в очередь.
    virtual size_t push(TItems const &items) = 0;
};


// todo: virtual public ISynchronized
/// \~english @brief An interface for popping items from an asynchronous queue.
/// \~russian @brief Интерфейс для извлечения элементов из асинхронной очереди.
template<typename TItem>
class IAsyncQueuePopper {
public:
    DEFINE_INTERFACE(IAsyncQueuePopper)

    /// \~english @brief A list of items.
    /// \~russian @brief Список элементов.
    typedef std::list<TItem> TItems;

    /// \~english @brief Pops items from the queue.
    /// \~russian @brief Извлекает элементы из очереди.
    /// \~english @param is_wait_required If true, waits until items are available.
    /// \~russian @param is_wait_required Если true, ожидает, пока элементы не станут доступны.
    virtual TItems pop(bool const &is_wait_required = true) = 0;
    /// \~english @brief Pops items from the queue with a timeout.
    /// \~russian @brief Извлекает элементы из очереди с тайм-аутом.
    /// \~english @param timeout The maximum time to wait for items.
    /// \~russian @param timeout Максимальное время ожидания элементов.
    virtual TItems pop(std::chrono::nanoseconds const &timeout) = 0;
};


// todo: rename to ThreadSafeQueue
/// \~english @brief An interface for an asynchronous queue.
/// \~russian @brief Интерфейс для асинхронной очереди.
template<typename TItem>
class IAsyncQueue:
    public IAsyncQueuePusher<TItem>,
    public IAsyncQueuePopper<TItem>
{
public:
    DEFINE_INTERFACE(IAsyncQueue)

    /// \~english @brief A list of items.
    /// \~russian @brief Список элементов.
    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    /// \~english @brief Interrupts the queue operations.
    /// \~russian @brief Прерывает операции с очередью.
    virtual void interrupt() = 0;
    /// \~english @brief Checks if the queue is empty.
    /// \~russian @brief Проверяет, пуста ли очередь.
    virtual bool empty() const = 0;
};


} // namespace iridium::threading


#endif // HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
