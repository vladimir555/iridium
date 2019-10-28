#include "listener.h"


IMPLEMENT_ENUM(iridium::io::Event::TType)


namespace iridium {
namespace io {


Event::Event(
    TType               const &type_,
    IStream::TSharedPtr const &stream_)
:
    type    (type_),
    stream  (stream_)
{}


} // io
} // iridium
