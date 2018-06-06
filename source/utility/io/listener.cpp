#include "listener.h"


IMPLEMENT_ENUM(utility::io::IListener::Event::TEvent)


namespace utility {
namespace io {


IListener::Event::Event(TEvent const &event_, IStream::TSharedPtr const &stream_)
:
    event   (event_),
    stream  (stream_)
{}


} // io
} // utility
