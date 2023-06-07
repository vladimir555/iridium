#include "event.h"


IMPLEMENT_ENUM(iridium::io::Event::TOperation)
IMPLEMENT_ENUM(iridium::io::Event::TStatus)


namespace iridium {
namespace io {


Event::Event(IStream::TSharedPtr const &stream_, TOperation const &operation_, TStatus const &status_)
:
    stream      (stream_),
    operation   (operation_),
    status      (status_)
{}


} // io
} // iridium
