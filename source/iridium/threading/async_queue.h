// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
#define HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE


#include <list>
#include <chrono>
#include "iridium/smart_ptr.h"


namespace iridium {
namespace threading {


// todo: must be SyncQueue
// todo: rename to ThreadSafeQueuePusher
// todo: virtual public ISynchronized
/// \~english @brief Interface for pushing items into an asynchronous queue.
///     This interface defines operations for adding one or more items to a queue,
///     which may be processed by a different thread.
/// \~russian @brief Интерфейс для добавления элементов в асинхронную очередь.
///     Этот интерфейс определяет операции для добавления одного или нескольких элементов в очередь,
///     которые могут обрабатываться другим потоком.
template<typename TItem>
class IAsyncQueuePusher {
public:
    DEFINE_INTERFACE(IAsyncQueuePusher)

    /// \~english @brief A list of items to be handled by the queue.
    /// \~russian @brief Список элементов для обработки очередью.
    typedef std::list<TItem> TItems;

    /// \~english @brief Pushes a single item into the queue.
    ///     Adds the specified item to the end of the queue.
    /// \~russian @brief Добавляет один элемент в очередь.
    ///     Добавляет указанный элемент в конец очереди.
    /// \~english @param item The item to be pushed into the queue.
    /// \~russian @param item Элемент для добавления в очередь.
    /// \~english @return The new size of the queue after the item is pushed.
    /// \~russian @return Новый размер очереди после добавления элемента.
    virtual size_t push(TItem  const &item)  = 0;

    /// \~english @brief Pushes multiple items into the queue.
    ///     Adds all items from the provided list to the end of the queue.
    /// \~russian @brief Добавляет несколько элементов в очередь.
    ///     Добавляет все элементы из предоставленного списка в конец очереди.
    /// \~english @param items A list of items to be pushed into the queue.
    /// \~russian @param items Список элементов для добавления в очередь.
    /// \~english @return The new size of the queue after the items are pushed.
    /// \~russian @return Новый размер очереди после добавления элементов.
    virtual size_t push(TItems const &items) = 0;
};


// todo: virtual public ISynchronized
/// \~english @brief Interface for popping items from an asynchronous queue.
///     This interface defines operations for retrieving items from a queue,
///     potentially waiting for items to become available.
/// \~russian @brief Интерфейс для извлечения элементов из асинхронной очереди.
///     Этот интерфейс определяет операции для извлечения элементов из очереди,
///     с возможностью ожидания появления элементов.
template<typename TItem>
class IAsyncQueuePopper {
public:
    DEFINE_INTERFACE(IAsyncQueuePopper)

    /// \~english @brief A list of items retrieved from the queue.
    /// \~russian @brief Список элементов, извлеченных из очереди.
    typedef std::list<TItem> TItems;

    /// \~english @brief Pops items from the queue.
    ///     Retrieves one or more items from the front of the queue.
    ///     The behavior regarding waiting depends on the implementation and the `is_do_wait` flag.
    /// \~russian @brief Извлекает элементы из очереди.
    ///     Извлекает один или несколько элементов из начала очереди.
    ///     Поведение относительно ожидания зависит от реализации и флага `is_do_wait`.
    /// \~english @param is_do_wait If true, the method may block until items are available. If false, it may return immediately if the queue is empty.
    /// \~russian @param is_do_wait Если true, метод может блокироваться до появления элементов. Если false, он может немедленно вернуться, если очередь пуста.
    /// \~english @return A list of items popped from the queue. The list might be empty depending on `is_do_wait` and queue state.
    /// \~russian @return Список элементов, извлеченных из очереди. Список может быть пустым в зависимости от `is_do_wait` и состояния очереди.
    virtual TItems pop(bool const &is_do_wait = true) = 0;

    /// \~english @brief Pops items from the queue with a timeout.
    ///     Attempts to retrieve items from the front of the queue, waiting up to the specified timeout duration.
    /// \~russian @brief Извлекает элементы из очереди с тайм-аутом.
    ///     Пытается извлечь элементы из начала очереди, ожидая в течение указанного периода времени.
    /// \~english @param timeout The maximum duration to wait for items to become available.
    /// \~russian @param timeout Максимальная продолжительность ожидания появления элементов.
    /// \~english @return A list of items popped from the queue. The list might be empty if no items become available within the timeout.
    /// \~russian @return Список элементов, извлеченных из очереди. Список может быть пустым, если элементы не появятся в течение тайм-аута.
    virtual TItems pop(std::chrono::nanoseconds const &timeout) = 0;
};


// todo: rename to ThreadSafeQueue
/// \~english @brief Interface for an asynchronous queue providing both push and pop operations.
///     This interface combines `IAsyncQueuePusher` and `IAsyncQueuePopper` and adds
///     functionality for queue interruption and state checking.
/// \~russian @brief Интерфейс для асинхронной очереди, предоставляющий операции добавления и извлечения.
///     Этот интерфейс объединяет `IAsyncQueuePusher` и `IAsyncQueuePopper` и добавляет
///     функциональность для прерывания операций очереди и проверки ее состояния.
template<typename TItem>
class IAsyncQueue:
    public IAsyncQueuePusher<TItem>,
    public IAsyncQueuePopper<TItem>
{
public:
    DEFINE_INTERFACE(IAsyncQueue)

    /// \~english @brief A list of items to be handled by the queue, inherited from pusher/popper.
    /// \~russian @brief Список элементов для обработки очередью, унаследованный от pusher/popper.
    typedef typename IAsyncQueuePusher<TItem>::TItems TItems;

    /// \~english @brief Interrupts any pending operations on the queue.
    ///     This method is typically used to unblock threads that are waiting on pop operations,
    ///     often as part of a shutdown sequence.
    /// \~russian @brief Прерывает все ожидающие операции в очереди.
    ///     Этот метод обычно используется для разблокировки потоков, ожидающих выполнения операций извлечения,
    ///     часто в рамках последовательности завершения работы.
    virtual void interrupt() = 0;

    /// \~english @brief Checks if the queue is currently empty.
    /// \~russian @brief Проверяет, пуста ли очередь в данный момент.
    /// \~english @return True if the queue contains no items, false otherwise.
    /// \~russian @return True, если очередь не содержит элементов, иначе false.
    virtual bool empty() const = 0;
};


} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_BC5EFE5F_A8A1_40A2_85B5_065C05F9F9EE
