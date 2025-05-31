/// \~english @brief Defines an interface for managing I/O contexts.
/// \~russian @brief Определяет интерфейс для управления контекстами ввода-вывода.
#ifndef HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
#define HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF


#include "context.h"    // For IContext, Event (via context.h -> event.h)
#include "protocol.h"   // For IProtocol
#include "event.h"      // For Event::TSharedPtr (explicitly, though also via context.h)
#include "multiplexer.h"// For IMultiplexer
// IStream is used by createContext, its definition is expected to be included transitively.
// For clarity: #include "stream.h" would typically be here if not for indirect inclusion.


namespace iridium {
namespace io {


/// \~english @brief Interface for managing a collection of I/O contexts (`IContext`).
///     A context manager is responsible for creating, destroying, and possibly distributing
///     I/O operations or streams among multiple contexts. This can be used to manage
///     resources, balance load, or handle different types of I/O tasks in separate contexts.
/// \~russian @brief Интерфейс для управления коллекцией контекстов ввода-вывода (`IContext`).
///     Менеджер контекстов отвечает за создание, уничтожение и, возможно, распределение
///     операций ввода-вывода или потоков между несколькими контекстами. Это может использоваться
///     для управления ресурсами, балансировки нагрузки или обработки различных типов задач ввода-вывода
///     в отдельных контекстах.
class IContextManager {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IContextManager)

    /// \~english @brief Creates a new I/O context, typically associated with an initial I/O stream and a protocol.
    ///     The `eventStream` parameter likely represents the primary stream (e.g., a new client connection)
    ///     for which this context is being created.
    /// \~russian @brief Создает новый контекст ввода-вывода, обычно связанный с начальным потоком ввода-вывода и протоколом.
    ///     Параметр `eventStream`, вероятно, представляет основной поток (например, новое клиентское соединение),
    ///     для которого создается этот контекст.
    /// \~english @param eventStream A shared pointer to an `IStream` that is the basis for this context, or an event source.
    ///     Despite the name "event", the type is `IStream::TSharedPtr`, suggesting it's a stream.
    /// \~russian @param eventStream Умный указатель на `IStream`, который является основой для этого контекста, или источник событий.
    ///     Несмотря на имя "event", тип параметра - `IStream::TSharedPtr`, что предполагает, что это поток.
    /// \~english @param protocol A shared pointer to an `IProtocol` to be used by this context for handling the stream.
    /// \~russian @param protocol Умный указатель на `IProtocol`, который будет использоваться этим контекстом для обработки потока.
    /// \~english @throws iridium::Exception or similar if context creation fails.
    /// \~russian @throws iridium::Exception или аналогичное, если создание контекста не удалось.
    virtual void    createContext (IStream::TSharedPtr  const &eventStream, IProtocol::TSharedPtr const &protocol) = 0;

    /// \~english @brief Removes (destroys or releases) an existing I/O context.
    ///     This function is responsible for cleaning up resources associated with the specified context.
    /// \~russian @brief Удаляет (уничтожает или освобождает) существующий контекст ввода-вывода.
    ///     Эта функция отвечает за очистку ресурсов, связанных с указанным контекстом.
    /// \~english @param context A shared pointer to the `IContext` to be removed.
    /// \~russian @param context Умный указатель на `IContext`, который необходимо удалить.
    /// \~english @throws iridium::Exception or similar if context removal fails.
    /// \~russian @throws iridium::Exception или аналогичное, если удаление контекста не удалось.
    virtual void    removeContext (IContext::TSharedPtr const &context) = 0;

    /// \~english @brief Acquires or selects an appropriate I/O context to handle a given event,
    ///     possibly associating it with a specific I/O multiplexer.
    ///     This could be used in a scenario where events arrive and need to be routed to a suitable context for processing.
    /// \~russian @brief Получает или выбирает подходящий контекст ввода-вывода для обработки данного события,
    ///     возможно, связывая его с определенным мультиплексором ввода-вывода.
    ///     Это может использоваться в сценарии, когда события поступают и должны быть направлены
    ///     в подходящий контекст для обработки.
    /// \~english @param event A shared pointer to the `Event` that needs to be processed.
    /// \~russian @param event Умный указатель на `Event`, которое необходимо обработать.
    /// \~english @param multiplexer A shared pointer to an `IMultiplexer` that might be associated with or used by the context.
    /// \~russian @param multiplexer Умный указатель на `IMultiplexer`, который может быть связан с контекстом или использоваться им.
    /// \~english @return A shared pointer to an `IContext` deemed suitable for handling the event.
    ///     May return `nullptr` or throw if no suitable context can be acquired.
    /// \~russian @return Умный указатель на `IContext`, считающийся подходящим для обработки события.
    ///     Может вернуть `nullptr` или выбросить исключение, если подходящий контекст не может быть получен.
    virtual IContext::TSharedPtr
    acquireContext(Event::TSharedPtr    const &event, IMultiplexer::TSharedPtr const &multiplexer) = 0;

    /// \~english @brief Releases an I/O context, possibly after it has finished its primary tasks.
    ///     This might not immediately destroy the context but could transition it to an inactive state
    ///     or trigger processing of any pending events before eventual removal.
    /// \~russian @brief Освобождает контекст ввода-вывода, возможно, после того как он завершил свои основные задачи.
    ///     Это может не приводить к немедленному уничтожению контекста, а переводить его в неактивное состояние
    ///     или инициировать обработку любых ожидающих событий перед окончательным удалением.
    /// \~english @param context A shared pointer to the `IContext` to be released.
    /// \~russian @param context Умный указатель на `IContext`, который необходимо освободить.
    /// \~english @return A list of `Event::TSharedPtr` which might be pending or unprocessed events from the released context.
    /// \~russian @return Список `Event::TSharedPtr`, который может содержать ожидающие или необработанные события из освобожденного контекста.
    virtual std::list<Event::TSharedPtr>
                    releaseContext(IContext::TSharedPtr const &context) = 0;

    /// \~english @brief Checks for and retrieves events related to outdated streams across all managed contexts.
    ///     This method likely iterates through its managed contexts, calling a similar method on each,
    ///     or directly inspects streams to identify those that are outdated or have timed out.
    /// \~russian @brief Проверяет и извлекает события, связанные с устаревшими потоками, во всех управляемых контекстах.
    ///     Этот метод, вероятно, итерирует по управляемым им контекстам, вызывая аналогичный метод для каждого,
    ///     или напрямую проверяет потоки для выявления устаревших или просроченных.
    /// \~english @return A list of shared pointers to events representing outdated streams or timeout notifications
    ///     from any of the managed contexts.
    /// \~russian @return Список умных указателей на события, представляющие устаревшие потоки или уведомления о тайм-ауте
    ///     из любого из управляемых контекстов.
    virtual std::list<Event::TSharedPtr>
                    checkOutdatedStreams() = 0;
};


} // io
} // iridium


#endif // HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
