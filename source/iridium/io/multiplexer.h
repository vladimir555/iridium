#ifndef HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h"

#include "event.h"

#include <chrono>


namespace iridium {
namespace io {


// todo: public IAsyncQueuePopper<IEvent::TSharedPtr>
// todo: rename IMultiplexer
class IMultiplexer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IMultiplexer)

    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    virtual std::list<IEvent::TSharedPtr> waitEvents() = 0;
    // todo: subscribe for IStreamReader and IStreamWriter
    virtual void subscribe  (IStream::TSharedPtr const &stream) = 0;
    virtual void unsubscribe(IStream::TSharedPtr const &stream) = 0;
};


} // io
} // iridium


#endif // HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E