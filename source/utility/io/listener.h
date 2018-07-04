#ifndef HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
#define HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0


#include "utility/smart_ptr.h"
#include "utility/enum.h"
#include "utility/pattern/initializable.h"

#include "stream.h"

#include <list>


namespace utility {
namespace io {


class IListener: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IListener)

    class Event {
    public:
        DEFINE_CREATE       (Event)
        DEFINE_ENUM         (TEvent, OPEN, CLOSE, READ, WRITE, ERROR)

        Event(TEvent const &event_, IStream::TSharedPtr const &stream_);
        virtual ~Event() = default;

        TEvent              event;
        IStream::TSharedPtr stream;
    };

    typedef std::list<Event::TSharedPtr> TEvents;

    virtual void add(IStream::TSharedPtr const &stream) = 0;
    virtual void del(IStream::TSharedPtr const &stream) = 0;
    virtual TEvents wait()  = 0;
};


} // io
} // utility


#endif // HEADER_LISTENER_2169E673_72F7_4B5C_B4A8_F06B40DF9BA0
