// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h"

#include "event.h"

#include <chrono>


namespace iridium::io {


// todo: public IAsyncQueuePopper<IEvent::TSharedPtr>
class IMultiplexer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IMultiplexer)
    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    virtual std::list<Event::TSharedPtr> waitEvents() = 0;
    virtual void subscribe  (IStream::TSharedPtr const &stream) = 0;
    virtual void unsubscribe(IStream::TSharedPtr const &stream) = 0;
    virtual void wake(Event::TSharedPtr const &event) = 0;
};


} // namespace iridium::io


#endif // HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
