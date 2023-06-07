#ifndef HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
#define HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF


#include "context.h"
#include "protocol.h"
#include "event.h"


namespace iridium {
namespace io {


class IContextManager {
public:
    DEFINE_INTERFACE(IContextManager)
    
    virtual void    createContext (IStream::TSharedPtr  const &event, IProtocol::TSharedPtr const &protocol) = 0;
    virtual void    removeContext (IContext::TSharedPtr const &context) = 0;
    virtual IContext::TSharedPtr
                    acquireContext(Event::TSharedPtr    const &event) = 0;
    virtual std::list<Event::TSharedPtr>
                    releaseContext(IContext::TSharedPtr const &context) = 0;
    virtual std::list<Event::TSharedPtr>
                    checkOutdatedStreams() = 0;
};


} // io
} // iridium


#endif // HEADER_CONTEXT_MANAGER_342147A2_0A13_4C1F_B1BD_7AD4D8B307AF
