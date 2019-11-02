// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
#define HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0


#include "iridium/smart_ptr.h"
#include "iridium/enum.h"
#include "iridium/pattern/initializable.h"

#include "stream.h"

#include <list>


namespace iridium {
namespace io {


class Event {
public:
    DEFINE_CREATE   (Event)
    DEFINE_ENUM     (TType, OPEN, CLOSE, READ, WRITE, ERROR)
    
    Event(
        TType               const &type,
        IStream::TSharedPtr const &stream);
    virtual ~Event() = default;
    
    TType               type;
    IStream::TSharedPtr stream;
};


class IListener: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IListener)

    typedef std::list<Event::TSharedPtr> TEvents;

    /// add stream for monitoring
    virtual void    add(IStream::TSharedPtr const &stream) = 0;
    /// del stream from monitoring set
    virtual void    del(IStream::TSharedPtr const &stream) = 0;
    /// waiting for new events
    virtual TEvents wait() = 0;
};


} // io
} // iridium


#endif // HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
