// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `IContextManager` interface for managing I/O contexts.
/// \~russian @file
/// @brief Определяет интерфейс `IContextManager` для управления контекстами ввода-вывода.

#ifndef HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
#define HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF


#include "context.h"    // For IContext, Event (via context.h -> event.h)
#include "protocol.h"   // For IProtocol
#include "event.h"      // For Event::TSharedPtr (explicitly, though also via context.h)
#include "multiplexer.h"// For IMultiplexer


namespace iridium::io {


// todo: rm, deprecated
class IContextManager {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IContextManager)

    virtual void
        createContext (IStream::TSharedPtr const &event, IProtocol::TSharedPtr const &protocol) = 0;
    // virtual void
    //     removeContext (IContext::TSharedPtr const &context) = 0;
    virtual IContext::TSharedPtr
        acquireContext(Event::TSharedPtr const &event, IMultiplexer::TSharedPtr const &multiplexer) = 0;
    virtual std::list<Event::TSharedPtr>
        releaseContext(IContext::TSharedPtr const &context, bool const &is_valid_context) = 0;
    virtual std::list<Event::TSharedPtr>
        checkOutdatedStreams() = 0;
};


} // namespace iridium::io


#endif // HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
