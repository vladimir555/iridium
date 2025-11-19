// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
#define HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47


#include "iridium/threading/thread.h"
#include "iridium/threading/worker.h"
#include "iridium/convertion/convert.h"

#include "worker.h"
#include "async_queue.h"

#include <list>


using iridium::convertion::convert;
using std::string;


namespace iridium::threading::implementation {


// ----- interface

/// \~english @brief A base class for worker pools.
/// \~russian @brief Базовый класс для пулов "работников".
class WorkerPoolBase {
public:
    /// \~english @brief Virtual destructor.
    /// \~russian @brief Виртуальный деструктор.
    virtual ~WorkerPoolBase() = default;

protected:
    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    WorkerPoolBase(std::string const &name);
    /// \~english @brief Waits for multiple statuses.
    /// \~russian @brief Ожидает несколько статусов.
    void waitForMultipleStatus(bool const &status);

    /// \~english @brief A list of threads in the pool.
    /// \~russian @brief Список потоков в пуле.
    std::list<IThread::TSharedPtr>  m_threads;

private:
    std::string m_name;
};

/// \~english @brief A worker pool that pushes items to a queue.
/// \~russian @brief Пул "работников", который добавляет элементы в очередь.
template<typename TItem>
class CWorkerPoolPusher: public IWorkerPusher<TItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPoolPusher)

    typedef typename IWorkerPusher<TItem>::TItems                           TItems;
    typedef std::list<typename IWorkerPusher<TItem>::IHandler::TSharedPtr>  THandlers;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPoolPusher(std::string const &name, THandlers const &handlers);

    /// \~english @brief Initializes the worker pool.
    /// \~russian @brief Инициализирует пул "работников".
    void initialize() override;
    /// \~english @brief Finalizes the worker pool.
    /// \~russian @brief Финализирует пул "работников".
    void finalize() override;

    /// \~english @brief Pushes an item to the queue.
    /// \~russian @brief Добавляет элемент в очередь.
    size_t push(TItem  const &item)  override;
    /// \~english @brief Pushes a list of items to the queue.
    /// \~russian @brief Добавляет список элементов в очередь.
    size_t push(TItems const &items) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
};

/// \~english @brief A worker pool that pops items from a queue.
/// \~russian @brief Пул "работников", который извлекает элементы из очереди.
template<typename TItem>
class CWorkerPoolPopper: public IWorkerPopper<TItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPoolPopper)

    typedef typename IAsyncQueuePopper<TItem>::TItems                       TItems;
    typedef std::list<typename IWorkerPopper<TItem>::IHandler::TSharedPtr>  THandlers;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPoolPopper(std::string const &name, THandlers const &handlers);

    /// \~english @brief Initializes the worker pool.
    /// \~russian @brief Инициализирует пул "работников".
    void initialize() override;
    /// \~english @brief Finalizes the worker pool.
    /// \~russian @brief Финализирует пул "работников".
    void finalize() override;

    /// \~english @brief Pops items from the queue.
    /// \~russian @brief Извлекает элементы из очереди.
    TItems pop(bool const &is_wait_required) override;
    /// \~english @brief Pops items from the queue with a timeout.
    /// \~russian @brief Извлекает элементы из очереди с тайм-аутом.
    TItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr m_queue;
};

/// \~english @brief A worker pool that processes items from an input queue and pushes them to an output queue.
/// \~russian @brief Пул "работников", который обрабатывает элементы из входной очереди и добавляет их в выходную очередь.
template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorkerPool: public IWorker<TInputItem, TOutputItem>, public WorkerPoolBase {
public:
    DEFINE_IMPLEMENTATION(CWorkerPool)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems                              TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems                             TOutputItems;
    typedef std::list<typename IWorker<TInputItem, TOutputItem>::IHandler::TSharedPtr>  THandlers;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPool(std::string const &name, THandlers const &handlers);

    /// \~english @brief Initializes the worker pool.
    /// \~russian @brief Инициализирует пул "работников".
    void initialize() override;
    /// \~english @brief Finalizes the worker pool.
    /// \~russian @brief Финализирует пул "работников".
    void finalize() override;

    /// \~english @brief Pushes an item to the input queue.
    /// \~russian @brief Добавляет элемент во входную очередь.
    size_t push(TInputItem  const &item)  override;
    /// \~english @brief Pushes a list of items to the input queue.
    /// \~russian @brief Добавляет список элементов во входную очередь.
    size_t push(TInputItems const &items) override;

    /// \~english @brief Pops items from the output queue.
    /// \~russian @brief Извлекает элементы из выходной очереди.
    TOutputItems pop(bool const &is_wait_required) override;
    /// \~english @brief Pops items from the output queue with a timeout.
    /// \~russian @brief Извлекает элементы из выходной очереди с тайм-аутом.
    TOutputItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    typename IAsyncQueue<TInputItem>::TSharedPtr    m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr   m_output_queue;
};


//----- implementation


template<typename TItem>
CWorkerPoolPusher<TItem>::CWorkerPoolPusher(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_queue         (CAsyncQueue<TItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(
            CThread::create(name + "[" + convert<string>(i++) + "]",
            CWorkerPusherRunnable<TItem>::create(handler, m_queue)));
}


template<typename TItem>
void CWorkerPoolPusher<TItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TItem>
void CWorkerPoolPusher<TItem>::finalize() {
    m_queue->interrupt();
    for (auto const &thread: m_threads)
        thread->finalize();
    m_queue->interrupt();
}


template<typename TItem>
size_t CWorkerPoolPusher<TItem>::push(TItem const &item) {
    return m_queue->push(item);
}


template<typename TItem>
size_t CWorkerPoolPusher<TItem>::push(TItems const &items) {
    return m_queue->push(items);
}


template<typename TItem>
CWorkerPoolPopper<TItem>::CWorkerPoolPopper(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_queue         (CAsyncQueue<TItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(
            CThread::create(name + "[" + convert<string>(i++) + "]",
            CWorkerPopperRunnable<TItem>::create(handler, m_queue)));
}


template<typename TItem>
void CWorkerPoolPopper<TItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TItem>
void CWorkerPoolPopper<TItem>::finalize() {
    m_queue->interrupt();
    for (auto const &thread: m_threads)
        thread->finalize();
    m_queue->interrupt();
}


template<typename TItem>
typename CWorkerPoolPopper<TItem>::TItems CWorkerPoolPopper<TItem>::pop(bool const &is_wait_required) {
    return m_queue->pop(is_wait_required); // ----->
}


template<typename TItem>
typename CWorkerPoolPopper<TItem>::TItems CWorkerPoolPopper<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    return m_queue->pop(timeout); // ----->
}


template<typename TInputItem, typename TOutputItem>
CWorkerPool<TInputItem, TOutputItem>::CWorkerPool(std::string const &name, THandlers const &handlers)
:
    WorkerPoolBase  (name),
    m_input_queue   (CAsyncQueue<TInputItem>::create()),
    m_output_queue  (CAsyncQueue<TOutputItem>::create())
{
    auto i = 0;
    for (auto const &handler: handlers)
        m_threads.push_back(CThread::create(
            name + "[" + convert<string>(i++) + "]",
            CWorkerRunnable<TInputItem, TOutputItem>::create(
                handler, m_input_queue, m_output_queue)));
}


template<typename TInputItem, typename TOutputItem>
void CWorkerPool<TInputItem, TOutputItem>::initialize() {
    for (auto const &thread: m_threads)
        thread->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorkerPool<TInputItem, TOutputItem>::finalize() {
    m_input_queue->interrupt();
    m_output_queue->interrupt();
    for (auto const &thread: m_threads)
        thread->finalize();
    m_output_queue->interrupt();
    m_input_queue->interrupt();
}


template<typename TInputItem, typename TOutputItem>
size_t CWorkerPool<TInputItem, TOutputItem>::push(TInputItem const &item) {
    return m_input_queue->push(item);
}


template<typename TInputItem, typename TOutputItem>
size_t CWorkerPool<TInputItem, TOutputItem>::push(TInputItems const &items) {
    return m_input_queue->push(items);
}


template<typename TInputItem, typename TOutputItem>
typename CWorkerPool<TInputItem, TOutputItem>::TOutputItems CWorkerPool<TInputItem, TOutputItem>::pop(bool const &is_wait_required) {
    return m_output_queue->pop(is_wait_required);
}


template<typename TInputItem, typename TOutputItem>
typename CWorkerPool<TInputItem, TOutputItem>::TOutputItems CWorkerPool<TInputItem, TOutputItem>::pop(std::chrono::nanoseconds const &timeout) {
    return m_output_queue->pop(timeout);
}


template<typename TIWorkerHandler, typename TCWorkerHandler, typename ... TArgs>
std::list<typename TIWorkerHandler::TSharedPtr> createHandlers(size_t const &count, TArgs ... args) {
    std::list<typename TIWorkerHandler::TSharedPtr> handlers;
    for (size_t i = 0; i < count; i++) {
        typename TIWorkerHandler::TSharedPtr handler = TCWorkerHandler::create(std::forward<TArgs>(args) ...);
        handlers.push_back(handler);
    }
    return handlers;
}


} // iridium::threading::implementation


#endif // HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
