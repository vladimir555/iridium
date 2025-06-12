#ifndef HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
#define HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB


#include "iridium/io/event.h"
#include "iridium/io/protocol.h"
#include "iridium/io/context_manager.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>
#include <unordered_set>


namespace iridium {
namespace io {
namespace implementation {


/// \~english @brief Concrete implementation of the `IContextManager` interface.
///     Manages the lifecycle of multiple `IContext` instances, associating them with streams
///     and handling their creation, acquisition, release, and removal.
///     It uses a mutex for thread-safe operations on its internal collections.
/// \~russian @brief Конкретная реализация интерфейса `IContextManager`.
///     Управляет жизненным циклом множества экземпляров `IContext`, связывая их с потоками
///     и обрабатывая их создание, получение, освобождение и удаление.
///     Использует мьютекс для потокобезопасных операций со своими внутренними коллекциями.
class CContextManager:
    public IContextManager,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CContextManager)
    
    /// \~english @brief Creates a new communication context for a given stream and protocol.
    ///     The new context is stored internally and associated with the stream.
    /// \~russian @brief Создает новый контекст обмена данными для указанного потока и протокола.
    ///     Новый контекст сохраняется внутри и связывается с потоком.
    /// \~english @param stream The stream for which to create the context.
    /// \~russian @param stream Поток, для которого создается контекст.
    /// \~english @param protocol The protocol to be used by the new context.
    /// \~russian @param protocol Протокол, который будет использоваться новым контекстом.
    /// \~english @throws iridium::Exception if a context for the given stream already exists or if context creation fails.
    /// \~russian @throws iridium::Exception, если контекст для данного потока уже существует или если создание контекста не удалось.
    void    createContext (IStream::TSharedPtr  const &stream, IProtocol::TSharedPtr const &protocol) override;

    /// \~english @brief Marks a context for removal. The actual removal and cleanup might be deferred.
    ///     This method adds the context to an internal list of contexts to be removed.
    /// \~russian @brief Помечает контекст для удаления. Фактическое удаление и очистка могут быть отложены.
    ///     Этот метод добавляет контекст во внутренний список контекстов для удаления.
    /// \~english @param context The context to be marked for removal.
    /// \~russian @param context Контекст, который должен быть помечен для удаления.
    void    removeContext (IContext::TSharedPtr const &context) override;

    /// \~english @brief Acquires a context associated with an event's stream, potentially creating it if it doesn't exist.
    ///     If a context for the event's stream exists, it is returned.
    ///     If not, and the event is an `Event::NEW_STREAM`, a new context is created using the event's stream and protocol.
    ///     The acquired context is marked as "acquired" to prevent concurrent modification or premature removal.
    /// \~russian @brief Получает контекст, связанный с потоком события, потенциально создавая его, если он не существует.
    ///     Если контекст для потока события существует, он возвращается.
    ///     В противном случае, и если событие является `Event::NEW_STREAM`, создается новый контекст с использованием потока и протокола события.
    ///     Полученный контекст помечается как "полученный" для предотвращения одновременного изменения или преждевременного удаления.
    /// \~english @param event The event whose stream is used to find or create a context.
    /// \~russian @param event Событие, поток которого используется для поиска или создания контекста.
    /// \~english @param multiplexer The multiplexer to associate with the stream if a new context is created and the stream needs to be managed.
    /// \~russian @param multiplexer Мультиплексор для связи с потоком, если создается новый контекст и поток нуждается в управлении.
    /// \~english @return A shared pointer to the acquired or newly created context, or `nullptr` if no context could be associated or created.
    /// \~russian @return Умный указатель на полученный или вновь созданный контекст, или `nullptr`, если контекст не удалось связать или создать.
    IContext::TSharedPtr
            acquireContext(Event::TSharedPtr    const &event, IMultiplexer::TSharedPtr const &multiplexer) override;

    /// \~english @brief Releases a previously acquired context.
    ///     Removes the context from the set of "acquired" contexts. If the context was marked for removal,
    ///     this might trigger its actual deletion and generate `Event::CLOSE_STREAM` events for associated streams.
    /// \~russian @brief Освобождает ранее полученный контекст.
    ///     Удаляет контекст из набора "полученных" контекстов. Если контекст был помечен для удаления,
    ///     это может инициировать его фактическое удаление и генерацию событий `Event::CLOSE_STREAM` для связанных потоков.
    /// \~english @param context The context to release.
    /// \~russian @param context Контекст для освобождения.
    /// \~english @return A list of events (e.g., `Event::CLOSE_STREAM`) generated as a result of releasing and potentially cleaning up the context.
    /// \~russian @return Список событий (например, `Event::CLOSE_STREAM`), сгенерированных в результате освобождения и потенциальной очистки контекста.
    std::list<Event::TSharedPtr>
            releaseContext(IContext::TSharedPtr const &context) override;

    /// \~english @brief Checks all managed contexts for outdated streams and collects any resulting close events.
    /// \~russian @brief Проверяет все управляемые контексты на наличие устаревших потоков и собирает все результирующие события закрытия.
    /// \~english @return A list of `Event::CLOSE_STREAM` events from all contexts that had outdated streams.
    /// \~russian @return Список событий `Event::CLOSE_STREAM` из всех контекстов, в которых были устаревшие потоки.
    std::list<Event::TSharedPtr>
            checkOutdatedStreams() override;
    
private:
    /// \~english @brief Maps a stream to its associated communication context. Used for quick lookup.
    /// \~russian @brief Сопоставляет поток с связанным с ним контекстом обмена данными. Используется для быстрого поиска.
    std::unordered_map<IStream::TSharedPtr, IContext::TSharedPtr>
        m_map_stream_context;

    /// \~english @brief Set of contexts that are currently "acquired" (in use).
    ///     This helps manage concurrent access and prevents deletion while a context is active.
    /// \~russian @brief Набор контекстов, которые в настоящее время "получены" (используются).
    ///     Это помогает управлять одновременным доступом и предотвращает удаление во время активности контекста.
    std::unordered_set<IContext::TSharedPtr>
        m_acquired_contexts;

    /// \~english @brief Set of all existing contexts managed by this manager.
    /// \~russian @brief Набор всех существующих контекстов, управляемых этим менеджером.
    std::unordered_set<IContext::TSharedPtr>
        m_contexts;

    /// \~english @brief Set of contexts that have been marked for removal and are pending actual deletion.
    ///     Deletion might be deferred until the context is released.
    /// \~russian @brief Набор контекстов, которые были помечены для удаления и ожидают фактического удаления.
    ///     Удаление может быть отложено до тех пор, пока контекст не будет освобожден.
    std::unordered_set<IContext::TSharedPtr>
        m_contexts_to_remove;
};


} // implementation
} // io
} // iridium


#endif // HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
