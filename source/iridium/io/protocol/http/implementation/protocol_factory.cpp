#include "protocol_factory.h"

#include "stream_handler.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


IStreamHandler::TSharedPtr CProtocolFactory::createStreamHandler(IStream::TSharedPtr const &stream) {
    return CStreamHandler::create(std::dynamic_pointer_cast<IStreamPort>(stream)); // ----->
}
   

} // implementation
} // http
} // protocol
} // io
} // iridium
