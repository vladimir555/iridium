/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
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