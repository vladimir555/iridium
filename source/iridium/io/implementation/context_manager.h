#ifndef HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
#define HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB


#include "iridium/io/event.h"
#include "iridium/io/protocol.h"
#include "iridium/io/context_manager.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>
#include <unordered_set>


namespace iridium::io::implementation {


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

    void
        createContext (IStream::TSharedPtr const &event, IProtocol::TSharedPtr const &protocol) override;
    // void
    //     removeContext (IContext::TSharedPtr const &context) override;
    IContext::TSharedPtr
        acquireContext(Event::TSharedPtr const &event, IMultiplexer::TSharedPtr const &multiplexer) override;
    std::list<Event::TSharedPtr>
        releaseContext(IContext::TSharedPtr const &context, bool const &is_valid_context) override;
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


} // iridium::io::implementation


#endif // HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
