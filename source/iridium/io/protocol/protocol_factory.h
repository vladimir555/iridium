// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
#define HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C


#include "iridium/smart_ptr.h"
#include "iridium/io/stream.h"
#include "iridium/io/transmitter.h"
#include "iridium/io/listener.h"


namespace iridium {
namespace io {
namespace protocol {


class IProtocolHandler {
public:
    DEFINE_INTERFACE(IProtocolHandler)
    // result: do continue or not
    // todo: rename redirectStreams
    virtual bool redirectStreams(ITransmitterStreams::TSharedPtr const &transmitter, Event::TSharedPtr const &event) = 0;
};


class IProtocolFactory {
public:
    DEFINE_INTERFACE(IProtocolFactory)
    virtual IProtocolHandler::TSharedPtr createProtocolHandler() = 0;
};


} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
