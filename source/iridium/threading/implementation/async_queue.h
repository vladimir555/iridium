// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CAsyncQueue<TItem>` class, an implementation of the `IAsyncQueue<TItem>` interface.
///     This class provides a thread-safe asynchronous queue using `iridium::threading::Synchronized` for internal locking.
/// \~russian @file
/// @brief Определяет класс `CAsyncQueue<TItem>`, реализацию интерфейса `IAsyncQueue<TItem>`.
///     Этот класс предоставляет потокобезопасную асинхронную очередь, используя `iridium::threading::Synchronized` для внутренней блокировки.

#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include "iridium/pattern/non_copyable.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/synchronized.h"
#include "iridium/logging/logger.h"

#include <atomic>


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes and functions related to threading and synchronization.
/// \~russian @brief Содержит классы и функции, связанные с многопоточностью и синхронизацией.
namespace threading {
/// \~english @brief Contains concrete implementations of the interfaces defined in the parent `threading` namespace.
/// \~russian @brief Содержит конкретные реализации интерфейсов, определенных в родительском пространстве имен `threading`.
namespace implementation {


// ----- class definidion


/// \~english @brief A concrete implementation of `IAsyncQueue<TItem>`.
///     This class provides a thread-safe queue using a `std::list` for item storage
///     and `iridium::threading::Synchronized<std::mutex>` for synchronization.
///     It is non-copyable.
/// \~russian @brief Конкретная реализация `IAsyncQueue<TItem>`.
///     Этот класс предоставляет потокобезопасную очередь, используя `std::list` для хранения элементов
///     и `iridium::threading::Synchronized<std::mutex>` для синхронизации.
///     Класс не является копируемым.
/// \~english @tparam TItem The type of items to be stored in the queue.
/// \~russian @tparam TItem Тип элементов, сохраняемых в очереди.
template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    private Synchronized<std::mutex>, // Uses Synchronized for thread safety
    private pattern::NonCopyable      // Explicitly non-copyable
{
public:
    /// \~english @brief Defines standard factory methods (e.g., `create()`) and smart pointer typedefs for `CAsyncQueue`.
    /// \~russian @brief Определяет стандартные фабричные методы (например, `create()`) и typedef-ы умных указателей для `CAsyncQueue`.
    DEFINE_IMPLEMENTATION(CAsyncQueue)

    /// \~english @brief Default constructor. Initializes the queue as empty.
    /// \~russian @brief Конструктор по умолчанию. Инициализирует очередь как пустую.
    CAsyncQueue();

    /// \~english @brief Pushes a single item to the back of the queue.
    ///     This operation is thread-safe.
    /// \~russian @brief Добавляет один элемент в конец очереди.
    ///     Эта операция является потокобезопасной.
    /// \~english @param item The item to be added to the queue.
    /// \~russian @param item Элемент для добавления в очередь.
    /// \~english @return The new size of the queue after the item is added.
    /// \~russian @return Новый размер очереди после добавления элемента.
    size_t push(TItem const &item) override;

    /// \~english @brief Pushes a list of items to the back of the queue.
    ///     Items are added in the order they appear in the input list. This operation is thread-safe.
    /// \~russian @brief Добавляет список элементов в конец очереди.
    ///     Элементы добавляются в том порядке, в котором они представлены во входном списке. Эта операция является потокобезопасной.
    /// \~english @param items A list of items to be added to the queue.
    /// \~russian @param items Список элементов для добавления в очередь.
    /// \~english @return The new size of the queue after the items are added.
    /// \~russian @return Новый размер очереди после добавления элементов.
    size_t push(std::list<TItem> const &items) override;

    /// \~english @brief Pops all items currently in the queue.
    ///     If `is_waiting` is true and the queue is empty, this method blocks until an item is pushed or `interrupt()` is called.
    ///     If `is_waiting` is false and the queue is empty, it returns an empty list immediately. This operation is thread-safe.
    /// \~russian @brief Извлекает все элементы, находящиеся в данный момент в очереди.
    ///     Если `is_waiting` истинно и очередь пуста, этот метод блокируется до тех пор, пока элемент не будет добавлен или не будет вызван `interrupt()`.
    ///     Если `is_waiting` ложно и очередь пуста, немедленно возвращается пустой список. Эта операция является потокобезопасной.
    /// \~english @param is_waiting If true, wait for items if the queue is empty. Defaults to true.
    /// \~russian @param is_waiting Если true, ожидать элементы, если очередь пуста. По умолчанию true.
    /// \~english @return A list containing all items popped from the queue. Can be empty.
    /// \~russian @return Список, содержащий все извлеченные из очереди элементы. Может быть пустым.
    std::list<TItem> pop(bool const &is_waiting = true) override;

    /// \~english @brief Pops all items currently in the queue, waiting up to a specified timeout.
    ///     If the queue is empty, this method blocks until an item is pushed, `interrupt()` is called, or the timeout expires.
    ///     This operation is thread-safe.
    /// \~russian @brief Извлекает все элементы, находящиеся в данный момент в очереди, с ожиданием до указанного тайм-аута.
    ///     Если очередь пуста, этот метод блокируется до тех пор, пока элемент не будет добавлен, не будет вызван `interrupt()` или не истечет тайм-аут.
    ///     Эта операция является потокобезопасной.
    /// \~english @param timeout The maximum duration to wait for items.
    /// \~russian @param timeout Максимальная продолжительность ожидания элементов.
    /// \~english @return A list containing all items popped from the queue. Can be empty if timeout occurs or interrupted.
    /// \~russian @return Список, содержащий все извлеченные из очереди элементы. Может быть пустым, если истек тайм-аут или произошло прерывание.
    std::list<TItem> pop(std::chrono::nanoseconds const &timeout) override;

    /// \~english @brief Interrupts any threads waiting on `pop()` operations.
    ///     This causes waiting `pop()` calls to return, typically with an empty list.
    ///     It also prevents future `pop()` calls from waiting if the queue is empty.
    /// \~russian @brief Прерывает любые потоки, ожидающие выполнения операций `pop()`.
    ///     Это приводит к тому, что ожидающие вызовы `pop()` возвращаются, обычно с пустым списком.
    ///     Это также предотвращает будущее ожидание вызовов `pop()`, если очередь пуста.
    void interrupt() override;

    /// \~english @brief Checks if the queue is currently empty.
    ///     This check is based on an atomic flag that is updated during push/pop operations.
    /// \~russian @brief Проверяет, пуста ли очередь в данный момент.
    ///     Эта проверка основана на атомарном флаге, который обновляется во время операций добавления/извлечения.
    /// \~english @return True if the queue is considered empty, false otherwise.
    /// \~russian @return True, если очередь считается пустой, иначе false.
    bool empty() const override;
    
private:
    /// \~english @brief The internal list used to store queue items. Access is synchronized.
    /// \~russian @brief Внутренний список, используемый для хранения элементов очереди. Доступ синхронизирован.
    std::list<TItem>    m_items;
//    std::atomic<bool>   m_is_waiting;
    /// \~english @brief Atomic flag indicating whether the queue is empty.
    ///     Used for a quick check without always acquiring the main lock.
    /// \~russian @brief Атомарный флаг, указывающий, пуста ли очередь.
    ///     Используется для быстрой проверки без необходимости всегда захватывать основную блокировку.
    std::atomic<bool>   m_is_empty;
};


// ----- class implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
//    m_is_waiting(true),
    m_is_empty  (true)
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    {
        LOCK_SCOPE();
        m_items.push_back(item);
        m_is_empty = m_items.empty();
        auto size  = m_items.size();

        return size; // ----->
    }


//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        m_items.push_back(item);
//        m_is_empty = m_items.empty();
//        auto size  = m_items.size();
//
//        l.unlock();
//        m_cv.notify_one();
//
//        return size; // ----->
//    }
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    {
        LOCK_SCOPE();

        m_items.insert(m_items.end(), items.begin(), items.end());
        m_is_empty = m_items.empty();

        return m_items.size();
    }

    
//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        m_items.insert(m_items.end(), items.begin(), items.end());
//        m_is_empty  = m_items.empty();
//        auto size   = m_items.size();
//
//        l.unlock();
//        m_cv.notify_one();
//
//        return size; // ----->
//    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_waiting) {
    {
        LOCK_SCOPE();
        if (m_is_empty && is_waiting)
            LOCK_SCOPE_TRY_WAIT({});

        m_is_empty = true;
        return std::move(m_items); // ----->
    }

//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//        while (m_is_waitable && m_is_empty && is_waiting)
//            m_cv.wait(l);
//
//        m_is_empty = true;
//        return std::move(m_items); // ----->
//    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    {
        LOCK_SCOPE();

        if (m_is_empty)
            LOCK_SCOPE_TRY_WAIT(timeout);

        m_is_empty = true;

        return std::move(m_items); // ----->
    }


//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        if (m_is_waitable && m_is_empty)
//            m_cv.wait_for(l, timeout);
//
//        m_is_empty = true;
//
//        return std::move(m_items); // ----->
//    }
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    Synchronized::interrupt();

//    {
//        m_is_waiting = false;
//        m_cv.notify_all();
//    }
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
