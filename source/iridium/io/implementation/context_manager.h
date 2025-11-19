#ifndef HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
#define HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB


#include "iridium/io/event.h"
#include "iridium/io/protocol.h"
#include "iridium/io/context_manager.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>
#include <unordered_set>


namespace iridium::io::implementation {


class CContextManager:
    public IContextManager,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CContextManager)

    void    createContext (IStream::TSharedPtr  const &event, IProtocol::TSharedPtr const &protocol) override;
    void    removeContext (IContext::TSharedPtr const &context) override;
    IContext::TSharedPtr
            acquireContext(Event::TSharedPtr    const &event, IMultiplexer::TSharedPtr const &multiplexer) override;
    std::list<Event::TSharedPtr>
            releaseContext(IContext::TSharedPtr const &context) override;
    std::list<Event::TSharedPtr>
            checkOutdatedStreams() override;

private:
    std::unordered_map<IStream::TSharedPtr, IContext::TSharedPtr>
        m_map_stream_context;
    std::unordered_set<IContext::TSharedPtr>
        m_acquired_contexts;
    std::unordered_set<IContext::TSharedPtr>
        m_contexts;
    std::unordered_set<IContext::TSharedPtr>
        m_contexts_to_remove;
};


} // iridium::io::implementation


#endif // HEADER_CONTEXT_MANAGER_741A7C4F_8CDD_47F0_9277_640033DD67AB
