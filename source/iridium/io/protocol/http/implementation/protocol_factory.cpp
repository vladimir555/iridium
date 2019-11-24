/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "protocol_factory.h"

#include "protocol_handler.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


IProtocolHandler::TSharedPtr CProtocolFactory::createProtocolHandler() {
    return CProtocolHandler::create(); // ----->
}
   

} // implementation
} // http
} // protocol
} // io
} // iridium
