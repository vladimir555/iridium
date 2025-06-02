// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines worker class implementations (`CWorkerPusher`, `CWorkerPopper`, `CWorker`)
///     that manage a thread to process items from asynchronous queues.
///     These classes provide concrete implementations for the `IWorkerPusher`, `IWorkerPopper`,
///     and `IWorker` interfaces respectively.
/// \~russian @file
/// @brief Определяет реализации рабочих классов (`CWorkerPusher`, `CWorkerPopper`, `CWorker`),
///     которые управляют потоком для обработки элементов из асинхронных очередей.
///     Эти классы предоставляют конкретные реализации для интерфейсов `IWorkerPusher`, `IWorkerPopper`
///     и `IWorker` соответственно.

#ifndef HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
#define HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6


#include "async_queue.h"

#include "iridium/threading/worker.h"

#include "thread.h"
#include "worker_handler.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes and functions related to threading and synchronization.
/// \~russian @brief Содержит классы и функции, связанные с многопоточностью и синхронизацией.
namespace threading {
/// \~english @brief Contains concrete implementations of the interfaces defined in the parent `threading` namespace.
/// \~russian @brief Содержит конкретные реализации интерфейсов, определенных в родительском пространстве имен `threading`.
namespace implementation {


/// \~english @brief Implements `IWorkerPusher` to push items to an internal queue processed by a dedicated worker thread.
/// \~russian @brief Реализует `IWorkerPusher` для добавления элементов во внутреннюю очередь, обрабатываемую выделенным рабочим потоком.
/// \~english @tparam TItem The type of items this worker pusher handles.
/// \~russian @tparam TItem Тип элементов, обрабатываемых этим добавителем элементов в очередь.
template<typename TItem>
class CWorkerPusher: public IWorkerPusher<TItem> {
public:
    /// \~english @brief Defines factory methods and smart pointer typedefs for `CWorkerPusher`.
    /// \~russian @brief Определяет фабричные методы и typedef-ы умных указателей для `CWorkerPusher`.
    DEFINE_IMPLEMENTATION(CWorkerPusher)

    /// \~english @brief Typedef for a list of items, inherited from `IAsyncQueuePusher`.
    /// \~russian @brief Typedef для списка элементов, унаследованный от `IAsyncQueuePusher`.
    typedef typename IAsyncQueuePusher<TItem>::TItems   TItems;
    /// \~english @brief Typedef for the handler interface used by the worker, inherited from `IWorkerPusher`.
    /// \~russian @brief Typedef для интерфейса обработчика, используемого рабочим потоком, унаследованный от `IWorkerPusher`.
    typedef typename IWorkerPusher<TItem>::IHandler     IHandler;

    /// \~english @brief Constructs a `CWorkerPusher`.
    /// \~russian @brief Конструирует `CWorkerPusher`.
    /// \~english @param name Name of the worker thread.
    /// \~russian @param name Имя рабочего потока.
    /// \~english @param handler Shared pointer to the handler that processes items from the queue.
    /// \~russian @param handler Умный указатель на обработчик, который обрабатывает элементы из очереди.
    CWorkerPusher(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker pusher, starting its internal thread.
    /// \~russian @brief Инициализирует добавитель элементов, запуская его внутренний поток.
    void initialize() override;
    /// \~english @brief Finalizes the worker pusher, interrupting the queue and joining its thread.
    /// \~russian @brief Финализирует добавитель элементов, прерывая очередь и присоединяя его поток.
    void finalize() override;

    /// \~english @brief Pushes a single item to the worker's queue.
    /// \~russian @brief Добавляет один элемент в очередь рабочего потока.
    /// \~english @param item The item to push.
    /// \~russian @param item Элемент для добавления.
    /// \~english @return The new size of the queue.
    /// \~russian @return Новый размер очереди.
    size_t push(TItem  const &item)  override;
    /// \~english @brief Pushes a list of items to the worker's queue.
    /// \~russian @brief Добавляет список элементов в очередь рабочего потока.
    /// \~english @param items The list of items to push.
    /// \~russian @param items Список элементов для добавления.
    /// \~english @return The new size of the queue.
    /// \~russian @return Новый размер очереди.
    size_t push(TItems const &items) override;

private:
    /// \~english @brief The internal asynchronous queue for items.
    /// \~russian @brief Внутренняя асинхронная очередь для элементов.
    typename IAsyncQueue<TItem>::TSharedPtr
        m_queue;
    /// \~english @brief The dedicated worker thread that processes items from `m_queue`.
    /// \~russian @brief Выделенный рабочий поток, который обрабатывает элементы из `m_queue`.
    IThread::TSharedPtr
        m_thread;
};


/// \~english @brief Implements `IWorkerPopper` to pop items processed by a dedicated worker thread from an internal queue.
/// \~russian @brief Реализует `IWorkerPopper` для извлечения элементов, обработанных выделенным рабочим потоком, из внутренней очереди.
/// \~english @tparam TItem The type of items this worker popper handles.
/// \~russian @tparam TItem Тип элементов, обрабатываемых этим извлекателем элементов из очереди.
template<typename TItem>
class CWorkerPopper: public IWorkerPopper<TItem> {
public:
    /// \~english @brief Defines factory methods and smart pointer typedefs for `CWorkerPopper`.
    /// \~russian @brief Определяет фабричные методы и typedef-ы умных указателей для `CWorkerPopper`.
    DEFINE_IMPLEMENTATION(CWorkerPopper)

    /// \~english @brief Typedef for a list of items, inherited from `IAsyncQueuePopper`.
    /// \~russian @brief Typedef для списка элементов, унаследованный от `IAsyncQueuePopper`.
    typedef typename IAsyncQueuePopper<TItem>::TItems   TItems;
    /// \~english @brief Typedef for the handler interface used by the worker, inherited from `IWorkerPopper`.
    /// \~russian @brief Typedef для интерфейса обработчика, используемого рабочим потоком, унаследованный от `IWorkerPopper`.
    typedef typename IWorkerPopper<TItem>::IHandler     IHandler;

    /// \~english @brief Constructs a `CWorkerPopper`.
    /// \~russian @brief Конструирует `CWorkerPopper`.
    /// \~english @param name Name of the worker thread.
    /// \~russian @param name Имя рабочего потока.
    /// \~english @param handler Shared pointer to the handler that generates/processes items to be placed in the queue.
    /// \~russian @param handler Умный указатель на обработчик, который генерирует/обрабатывает элементы для помещения в очередь.
    CWorkerPopper(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker popper, starting its internal thread.
    /// \~russian @brief Инициализирует извлекатель элементов, запуская его внутренний поток.
    void initialize() override;
    /// \~english @brief Finalizes the worker popper, interrupting the queue and joining its thread.
    /// \~russian @brief Финализирует извлекатель элементов, прерывая очередь и присоединяя его поток.
    void finalize() override;

    /// \~english @brief Pops items from the worker's output queue.
    /// \~russian @brief Извлекает элементы из выходной очереди рабочего потока.
    /// \~english @param is_do_wait If true, wait for items if the queue is empty.
    /// \~russian @param is_do_wait Если true, ожидать элементы, если очередь пуста.
    /// \~english @return A list of items popped from the queue.
    /// \~russian @return Список элементов, извлеченных из очереди.
    TItems pop(bool const &is_do_wait) override;
    /// \~english @brief Pops items from the worker's output queue with a timeout.
    /// \~russian @brief Извлекает элементы из выходной очереди рабочего потока с тайм-аутом.
    /// \~english @param timeout Maximum duration to wait for items.
    /// \~russian @param timeout Максимальная продолжительность ожидания элементов.
    /// \~english @return A list of items popped from the queue.
    /// \~russian @return Список элементов, извлеченных из очереди.
    TItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    /// \~english @brief The internal asynchronous queue from which items are popped.
    /// \~russian @brief Внутренняя асинхронная очередь, из которой извлекаются элементы.
    typename IAsyncQueue<TItem>::TSharedPtr
        m_queue;
    /// \~english @brief The dedicated worker thread that produces items and puts them into `m_queue`.
    /// \~russian @brief Выделенный рабочий поток, который производит элементы и помещает их в `m_queue`.
    IThread::TSharedPtr
        m_thread;
};


/// \~english @brief Implements `IWorker` to process items from an input queue and place results in an output queue, using a dedicated worker thread.
/// \~russian @brief Реализует `IWorker` для обработки элементов из входной очереди и помещения результатов в выходную очередь, используя выделенный рабочий поток.
/// \~english @tparam TInputItem The type of items in the input queue.
/// \~russian @tparam TInputItem Тип элементов во входной очереди.
/// \~english @tparam TOutputItem The type of items in the output queue (defaults to `TInputItem`).
/// \~russian @tparam TOutputItem Тип элементов в выходной очереди (по умолчанию `TInputItem`).
template<typename TInputItem, typename TOutputItem = TInputItem>
class CWorker: public IWorker<TInputItem, TOutputItem> {
public:
    /// \~english @brief Defines factory methods and smart pointer typedefs for `CWorker`.
    /// \~russian @brief Определяет фабричные методы и typedef-ы умных указателей для `CWorker`.
    DEFINE_IMPLEMENTATION(CWorker)

    /// \~english @brief Typedef for a list of input items.
    /// \~russian @brief Typedef для списка входных элементов.
    typedef typename IAsyncQueuePusher<TInputItem>::TItems      TInputItems;
    /// \~english @brief Typedef for a list of output items.
    /// \~russian @brief Typedef для списка выходных элементов.
    typedef typename IAsyncQueuePusher<TOutputItem>::TItems     TOutputItems;
    /// \~english @brief Typedef for the handler interface used by the worker.
    /// \~russian @brief Typedef для интерфейса обработчика, используемого рабочим потоком.
    typedef typename IWorker<TInputItem, TOutputItem>::IHandler IHandler;

    /// \~english @brief Constructs a `CWorker`.
    /// \~russian @brief Конструирует `CWorker`.
    /// \~english @param name Name of the worker thread.
    /// \~russian @param name Имя рабочего потока.
    /// \~english @param handler Shared pointer to the handler that processes input items to produce output items.
    /// \~russian @param handler Умный указатель на обработчик, который обрабатывает входные элементы для получения выходных элементов.
    CWorker(std::string const &name, typename IHandler::TSharedPtr const &handler);

    /// \~english @brief Initializes the worker, starting its internal thread.
    /// \~russian @brief Инициализирует рабочий поток, запуская его внутренний поток.
    void initialize() override;
    /// \~english @brief Finalizes the worker, interrupting input/output queues and joining its thread.
    /// \~russian @brief Финализирует рабочий поток, прерывая входные/выходные очереди и присоединяя его поток.
    void finalize() override;

    /// \~english @brief Pushes a single item to the worker's input queue.
    /// \~russian @brief Добавляет один элемент во входную очередь рабочего потока.
    /// \~english @param item The item to push.
    /// \~russian @param item Элемент для добавления.
    /// \~english @return The new size of the input queue.
    /// \~russian @return Новый размер входной очереди.
    size_t push(TInputItem  const &item) override;
    /// \~english @brief Pushes a list of items to the worker's input queue.
    /// \~russian @brief Добавляет список элементов во входную очередь рабочего потока.
    /// \~english @param items The list of items to push.
    /// \~russian @param items Список элементов для добавления.
    /// \~english @return The new size of the input queue.
    /// \~russian @return Новый размер входной очереди.
    size_t push(TInputItems const &items) override;

    /// \~english @brief Pops items from the worker's output queue.
    /// \~russian @brief Извлекает элементы из выходной очереди рабочего потока.
    /// \~english @param is_do_wait If true, wait for items if the output queue is empty.
    /// \~russian @param is_do_wait Если true, ожидать элементы, если выходная очередь пуста.
    /// \~english @return A list of items popped from the output queue.
    /// \~russian @return Список элементов, извлеченных из выходной очереди.
    TOutputItems pop(bool const &is_do_wait) override;
    /// \~english @brief Pops items from the worker's output queue with a timeout.
    /// \~russian @brief Извлекает элементы из выходной очереди рабочего потока с тайм-аутом.
    /// \~english @param timeout Maximum duration to wait for items.
    /// \~russian @param timeout Максимальная продолжительность ожидания элементов.
    /// \~english @return A list of items popped from the output queue.
    /// \~russian @return Список элементов, извлеченных из выходной очереди.
    TOutputItems pop(std::chrono::nanoseconds const &timeout) override;

private:
    /// \~english @brief The internal asynchronous queue for input items.
    /// \~russian @brief Внутренняя асинхронная очередь для входных элементов.
    typename IAsyncQueue<TInputItem>::TSharedPtr
        m_input_queue;
    /// \~english @brief The internal asynchronous queue for output items.
    /// \~russian @brief Внутренняя асинхронная очередь для выходных элементов.
    typename IAsyncQueue<TOutputItem>::TSharedPtr
        m_output_queue;
    /// \~english @brief The dedicated worker thread that processes items from input to output queue.
    /// \~russian @brief Выделенный рабочий поток, который обрабатывает элементы из входной в выходную очередь.
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
typename CWorkerPopper<TItem>::TItems CWorkerPopper<TItem>::pop(bool const &is_do_wait) {
    return m_queue->pop(is_do_wait);
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
typename CWorker<TInputItem, TOutputItem>::TOutputItems CWorker<TInputItem, TOutputItem>::pop(bool const &is_do_wait) {
//    return m_output_queue->pop(is_do_wait);
//    std::printf("worker pop wait ...\n");
    auto result = m_input_queue->pop(is_do_wait);
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


} // implementation
} // threading
} // iridium


#endif // HEADER_WORKER_62A45EF9_ADAA_4252_940F_C9952EAFD0A6
