// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
#define HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C


#include "iridium/smart_ptr.h"
#include "iridium/io/stream.h"
#include "iridium/io/stream_handler.h"


namespace iridium {
namespace io {
namespace protocol {


class IProtocolFactory {
public:
    DEFINE_INTERFACE(IProtocolFactory)
    virtual IStreamHandler::TSharedPtr createStreamHandler(IStream::TSharedPtr const &stream) = 0;
};


} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
