// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
#define HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6


#include "async_queue.h"

#include "iridium/threading/worker.h"

#include "thread.h"
#include "worker_handler.h"


namespace iridium::threading::implementation {


/// \~english @brief A worker that pushes items to a queue.
/// \~russian @brief "Работник", который добавляет элементы в очередь.
template<typename TItem>
class CWorkerPusher: public IWorkerPusher<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorkerPusher)

    typedef typename IAsyncQueuePusher<TItem>::TItems   TItems;
    typedef typename IWorkerPusher<TItem>::IHandler     IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPusher(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

    /// \~english @brief Pushes an item to the queue.
    /// \~russian @brief Добавляет элемент в очередь.
    size_t push(TItem  const &item)  override;
    /// \~english @brief Pushes a list of items to the queue.
    /// \~russian @brief Добавляет список элементов в очередь.
    size_t push(TItems const &items) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr
        m_queue;
    IThread::TSharedPtr
        m_thread;
};

/// \~english @brief A worker that pops items from a queue.
/// \~russian @brief "Работник", который извлекает элементы из очереди.
template<typename TItem>
class CWorkerPopper: public IWorkerPopper<TItem> {
public:
    DEFINE_IMPLEMENTATION(CWorkerPopper)

    typedef typename IAsyncQueuePopper<TItem>::TItems   TItems;
    typedef typename IWorkerPopper<TItem>::IHandler     IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPopper(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

    /// \~english @brief Pops items from the queue.
    /// \~russian @brief Извлекает элементы из очереди.
    TItems pop(bool const &is_wait_required) override;
    /// \~english @brief Pops items from the queue with a timeout.
    /// \~russian @brief Извлекает элементы из очереди с тайм-аутом.
    TItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr
        m_queue;
    IThread::TSharedPtr
        m_thread;
};

/// \~english @brief A worker that processes items from an input queue and pushes them to an output queue.
/// \~russian @brief "Работник", который обрабатывает элементы из входной очереди и добавляет их в выходную очередь.
template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorker: public IWorker<TInputItem, TOutputItem> {
public:
    DEFINE_IMPLEMENTATION(CWorker)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems      TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems     TOutputItems;
    typedef typename IWorker<TInputItem, TOutputItem>::IHandler IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorker(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

    /// \~english @brief Pushes an item to the input queue.
    /// \~russian @brief Добавляет элемент во входную очередь.
    size_t push(TInputItem  const &item) override;
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
    typename IAsyncQueue<TInputItem>::TSharedPtr
        m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr
        m_output_queue;
    IThread::TSharedPtr
        m_thread;
};


//----- implementation


template<typename TItem>
CWorkerPusher<TItem>::CWorkerPusher(std::string const &name, typename IHandler::TSharedPtr const &handler)
:
    m_queue     (CAsyncQueue<TItem>::create()),
    m_thread    (CThread::create(name, CWorkerPusherRunnable<TItem>::create(handler, m_queue)))
{}


template<typename TItem>
void CWorkerPusher<TItem>::initialize() {
    m_thread->initialize();
}


template<typename TItem>
void CWorkerPusher<TItem>::finalize() {
    m_queue->interrupt();
    m_thread->finalize();
    m_queue->interrupt();
}


template<typename TItem>
size_t CWorkerPusher<TItem>::push(TItem const &item) {
    return m_queue->push(item);
}


template<typename TItem>
size_t CWorkerPusher<TItem>::push(TItems const &items) {
    return m_queue->push(items);
}


template<typename TItem>
CWorkerPopper<TItem>::CWorkerPopper(std::string const &name, typename IHandler::TSharedPtr const &handler)
:
    m_queue     (CAsyncQueue<TItem>::create()),
    m_thread    (CThread::create(name, CWorkerPopperRunnable<TItem>::create(handler, m_queue)))
{}


template<typename TItem>
void CWorkerPopper<TItem>::initialize() {
    m_thread->initialize();
}


template<typename TItem>
void CWorkerPopper<TItem>::finalize() {
    m_queue->interrupt();
    m_thread->finalize();
    m_queue->interrupt();
}


template<typename TItem>
typename CWorkerPopper<TItem>::TItems CWorkerPopper<TItem>::pop(bool const &is_wait_required) {
    return m_queue->pop(is_wait_required);
}


template<typename TItem>
typename CWorkerPopper<TItem>::TItems CWorkerPopper<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    return m_queue->pop(timeout);
}


template<typename TInputItem, typename TOutputItem>
CWorker<TInputItem, TOutputItem>::CWorker(
    std::string const &name,
    typename IHandler::TSharedPtr const &handler)
:
    m_input_queue   (CAsyncQueue<TInputItem>::create()),
    m_output_queue  (CAsyncQueue<TOutputItem>::create()),
    m_thread        (CThread::create(name, CWorkerRunnable<TInputItem, TOutputItem>::create(handler, m_input_queue, m_output_queue)))
{}


template<typename TInputItem, typename TOutputItem>
void CWorker<TInputItem, TOutputItem>::initialize() {
    m_thread->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorker<TInputItem, TOutputItem>::finalize() {
    m_input_queue->interrupt();
    m_output_queue->interrupt();
    m_thread->finalize();
    m_output_queue->interrupt();
    m_input_queue->interrupt();
}


template<typename TInputItem, typename TOutputItem>
size_t CWorker<TInputItem, TOutputItem>::push(TInputItem const &item) {
//    return m_input_queue->push(item);
//    std::printf("worker push item ...\n");
    auto result = m_input_queue->push(item);
//    std::printf("worker push item OK\n");
    return result;
}


template<typename TInputItem, typename TOutputItem>
size_t CWorker<TInputItem, TOutputItem>::push(TInputItems const &items) {
//    return m_input_queue->push(items);
//    std::printf("worker push items ...\n");
    auto result = m_input_queue->push(items);
//    std::printf("worker push items OK\n");
    return result;
}


template<typename TInputItem, typename TOutputItem>
typename CWorker<TInputItem, TOutputItem>::TOutputItems CWorker<TInputItem, TOutputItem>::pop(bool const &is_wait_required) {
//    return m_output_queue->pop(is_wait_required);
//    std::printf("worker pop wait ...\n");
    auto result = m_input_queue->pop(is_wait_required);
//    std::printf("worker pop wait OK\n");
    return result;
}


template<typename TInputItem, typename TOutputItem>
typename CWorker<TInputItem, TOutputItem>::TOutputItems CWorker<TInputItem, TOutputItem>::pop(std::chrono::nanoseconds const &timeout) {
//    return m_output_queue->pop(timeout);
//    std::printf("worker pop timeout ...\n");
    auto result = m_input_queue->pop(timeout);
//    std::printf("worker pop timeout OK\n");
    return result;
}


} // iridium::threading::implementation


#endif // HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
