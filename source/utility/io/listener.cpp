#include "listener.h"


IMPLEMENT_ENUM(utility::io::Event::TType)


namespace utility {
namespace io {


Event::Event(
    TType               const &type_,
    IStream::TSharedPtr const &stream_)
:
    type    (type_),
    stream  (stream_)
{}


} // io
} // utility
