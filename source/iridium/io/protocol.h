#ifndef HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB
#define HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB


#include "context.h"
#include "event.h"


namespace iridium {
namespace io {


class IProtocol {
public:
    DEFINE_INTERFACE(IProtocol)
    virtual bool control(Event::TSharedPtr const &event, IPipeManager::TSharedPtr const &pipe_manager) = 0;
};


} // io
} // iridium


#endif // HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB
