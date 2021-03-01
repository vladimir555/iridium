// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
#define HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0


#include "iridium/smart_ptr.h"
#include "iridium/enum.h"
#include "iridium/pattern/initializable.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/synchronized.h"
#include "iridium/pattern/observable.h"

#include "stream.h"

#include <list>


namespace iridium {
namespace io {


class IListenerStreams {
public:
    DEFINE_INTERFACE(IListenerStreams)
    /// add stream for monitoring
    virtual void    add(IStream::TSharedPtr const &stream) = 0;
    /// del stream from monitoring set
    virtual void    del(IStream::TSharedPtr const &stream) = 0;
};


// synchronized
class IListener: public pattern::IInitializable, public IListenerStreams {
public:
    DEFINE_INTERFACE(IListener)

    struct Event {
        DEFINE_CREATE   (Event)
        DEFINE_ENUM     (TType, NONE, OPEN, CLOSE, READ, WRITE, EOF_, ERROR)

        TType                   type;
        IStream::TSharedPtr     stream;
        IListener::TSharedPtr   listener;
    };

    typedef std::list<Event::TSharedPtr> TEvents;

//    /// add stream for monitoring
//    virtual void    add(IStream::TSharedPtr const &stream) = 0;
//    /// del stream from monitoring set
//    virtual void    del(IStream::TSharedPtr const &stream) = 0;
    /// waiting for new events
    virtual TEvents wait() = 0;
};


} // io
} // iridium


#endif // HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
