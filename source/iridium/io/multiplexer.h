// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h"

#include "event.h"

#include <chrono>


namespace iridium {
namespace io {


// todo: public IAsyncQueuePopper<IEvent::TSharedPtr>
class IMultiplexer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IMultiplexer)

    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    // no throw
    virtual std::list<IEvent::TSharedPtr> waitEvents() = 0;
    // todo: subscribe for IStreamReader and IStreamWriter
    virtual void subscribe  (IStream::TConstSharedPtr const &stream) = 0;
    // finalize and unsubscribe stream
    virtual void unsubscribe(IStream::TConstSharedPtr const &stream) = 0;
};


} // io
} // iridium


#endif // HEADER_EVENT_PROVIDER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
