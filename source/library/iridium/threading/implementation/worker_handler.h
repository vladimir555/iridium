// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33
#define HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33


#include "iridium/threading/runnable.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/async_queue.h"


namespace iridium::threading::implementation {


/// \~english @brief A runnable for a worker that pushes items to a queue.
/// \~russian @brief "Runnable" для "работника", который добавляет элементы в очередь.
template<typename TItem>
class CWorkerPusherRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerPusherRunnable)

    typedef typename IAsyncQueuePusher<TItem>::TItems   TItems;
    typedef typename IWorkerPusher<TItem>::IHandler     IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPusherRunnable(
        typename IHandler::TSharedPtr const &handler,
        typename IAsyncQueue<TItem>::TSharedPtr const &queue);

    /// \~english @brief The main execution loop for the worker.
    /// \~russian @brief Основной цикл выполнения для "работника".
    void run(std::atomic<bool> &is_running) override;
    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    typename IHandler::TSharedPtr                   m_handler;
};

/// \~english @brief A runnable for a worker that pops items from a queue.
/// \~russian @brief "Runnable" для "работника", который извлекает элементы из очереди.
template<typename TItem>
class CWorkerPopperRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerPopperRunnable)

    typedef typename IAsyncQueuePopper<TItem>::TItems   TItems;
    typedef typename IWorkerPopper<TItem>::IHandler     IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerPopperRunnable(
        typename IHandler::TSharedPtr           const &handler,
        typename IAsyncQueue<TItem>::TSharedPtr const &queue);

    /// \~english @brief The main execution loop for the worker.
    /// \~russian @brief Основной цикл выполнения для "работника".
    void run(std::atomic<bool> &is_running) override;
    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

private:
    typename IAsyncQueue<TItem>::TSharedPtr         m_queue;
    typename IHandler::TSharedPtr                   m_handler;
};

/// \~english @brief A runnable for a worker that processes items.
/// \~russian @brief "Runnable" для "работника", который обрабатывает элементы.
template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorkerRunnable: public IRunnable {
public:
    DEFINE_IMPLEMENTATION(CWorkerRunnable)

    typedef typename IAsyncQueuePusher<TInputItem>::TItems      TInputItems;
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems     TOutputItems;
    typedef typename IWorker<TInputItem, TOutputItem>::IHandler IHandler;

    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    CWorkerRunnable(
        typename IHandler::TSharedPtr                   const &handler,
        typename IAsyncQueue<TInputItem>::TSharedPtr    const &m_input_queue,
        typename IAsyncQueue<TOutputItem>::TSharedPtr   const &m_output_queue);

    /// \~english @brief The main execution loop for the worker.
    /// \~russian @brief Основной цикл выполнения для "работника".
    void run(std::atomic<bool> &is_running) override;
    /// \~english @brief Initializes the worker.
    /// \~russian @brief Инициализирует "работника".
    void initialize() override;
    /// \~english @brief Finalizes the worker.
    /// \~russian @brief Финализирует "работника".
    void finalize() override;

private:
    typename IAsyncQueue<TInputItem>::TSharedPtr    m_input_queue;
    typename IAsyncQueue<TOutputItem>::TSharedPtr   m_output_queue;
    typename IHandler::TSharedPtr                   m_handler;
};


//----- implementation


template<typename TItem>
CWorkerPusherRunnable<TItem>::CWorkerPusherRunnable(
    typename IHandler::TSharedPtr const &handler,
    typename IAsyncQueue<TItem>::TSharedPtr const &queue)
:
    m_queue     (queue),
    m_handler   (handler)
{}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto items = m_queue->pop(is_running);
        if (!items.empty())
            m_handler->handle(items);
    }
}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::initialize() {
    m_handler->initialize();
}


template<typename TItem>
void CWorkerPusherRunnable<TItem>::finalize() {
    m_handler->finalize();
}


template<typename TItem>
CWorkerPopperRunnable<TItem>::CWorkerPopperRunnable(
    typename IHandler::TSharedPtr const &handler,
    typename IAsyncQueue<TItem>::TSharedPtr const &queue)
:
    m_queue     (queue),
    m_handler   (handler)
{}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::run(std::atomic<bool> &is_running) {
    while (is_running)
        m_queue->push(m_handler->handle());
}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::initialize() {
    m_handler->initialize();
}


template<typename TItem>
void CWorkerPopperRunnable<TItem>::finalize() {
    m_handler->finalize();
}


template<typename TInputItem, typename TOutputItem>
CWorkerRunnable<TInputItem, TOutputItem>::CWorkerRunnable(
    typename IHandler::TSharedPtr                   const &handler,
    typename IAsyncQueue<TInputItem>::TSharedPtr    const &input_queue,
    typename IAsyncQueue<TOutputItem>::TSharedPtr   const &output_queue)
:
    m_input_queue   (input_queue),
    m_output_queue  (output_queue),
    m_handler       (handler)
{}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::run(std::atomic<bool> &is_running) {
    while (is_running)
        m_output_queue->push(m_handler->handle(m_input_queue->pop(is_running)));
}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::initialize() {
    m_handler->initialize();
}


template<typename TInputItem, typename TOutputItem>
void CWorkerRunnable<TInputItem, TOutputItem>::finalize() {
    m_handler->finalize();
}


} // iridium::threading::implementation


#endif // HEADER_WORKER_HANDLER_D977DE0C_D1EF_4571_9709_FC454A816A33
