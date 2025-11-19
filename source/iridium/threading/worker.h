// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
#define HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76


#include "iridium/smart_ptr.h"

#include "iridium/threading/async_queue.h"
#include "iridium/pattern/initializable.h"

#include <string>


namespace iridium::threading {


/// \~english @brief An interface for a worker that pushes items to a queue.
/// \~russian @brief Интерфейс для "работника", который добавляет элементы в очередь.
template<typename TItem>
class IWorkerPusher:
    public virtual IAsyncQueuePusher<TItem>,
    public virtual pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IWorkerPusher);
    /// \~english @brief An interface for a handler that processes items.
    /// \~russian @brief Интерфейс для обработчика, который обрабатывает элементы.
    class IHandler: public pattern::IInitializable {
    public:
        /// \~english @brief A list of input items.
        /// \~russian @brief Список входных элементов.
        using TInputItems = typename IAsyncQueuePusher<TItem>::TItems;

        DEFINE_INTERFACE(IHandler)
        /// \~english @brief Handles a list of items.
        /// \~russian @brief Обрабатывает список элементов.
        virtual void handle(TInputItems const &items) = 0;
    };
};

/// \~english @brief An interface for a worker that pops items from a queue.
/// \~russian @brief Интерфейс для "работника", который извлекает элементы из очереди.
template<typename TItem>
class IWorkerPopper:
    public virtual IAsyncQueuePopper<TItem>,
    public virtual pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IWorkerPopper)
    /// \~english @brief An interface for a handler that provides items.
    /// \~russian @brief Интерфейс для обработчика, который предоставляет элементы.
    class IHandler: public pattern::IInitializable {
    public:
        /// \~english @brief A list of output items.
        /// \~russian @brief Список выходных элементов.
        using TOutputItems = typename IAsyncQueuePopper<TItem>::TItems;

        DEFINE_INTERFACE(IHandler)
        /// \~english @brief Handles the provision of items.
        /// \~russian @brief Обрабатывает предоставление элементов.
        virtual TOutputItems handle() = 0;
    };
};

/// \~english @brief An interface for a worker that processes items, taking input and producing output.
/// \~russian @brief Интерфейс для "работника", который обрабатывает элементы, принимая входные данные и производя выходные.
template<typename TInputItem, typename TOutputItem = TInputItem>
class IWorker:
    public IWorkerPusher<TInputItem>,
    public IWorkerPopper<TOutputItem>
{
public:
    DEFINE_INTERFACE(IWorker)
    /// \~english @brief An interface for a handler that processes items.
    /// \~russian @brief Интерфейс для обработчика, который обрабатывает элементы.
    class IHandler: public pattern::IInitializable {
    public:
        /// \~english @brief A list of input items.
        /// \~russian @brief Список входных элементов.
        using TInputItems   = typename IWorkerPusher<TInputItem>::IHandler::TInputItems;
        /// \~english @brief A list of output items.
        /// \~russian @brief Список выходных элементов.
        using TOutputItems  = typename IWorkerPopper<TOutputItem>::IHandler::TOutputItems;

        DEFINE_INTERFACE(IHandler)
        /// \~english @brief Handles a list of input items and returns a list of output items.
        /// \~russian @brief Обрабатывает список входных элементов и возвращает список выходных элементов.
        virtual TOutputItems handle(TInputItems const &items) = 0;
    };
};


} // namespace iridium::threading


#endif // HEADER_WORKER_443BFEC7_C32B_4600_A0E9_D416C24C3F76
