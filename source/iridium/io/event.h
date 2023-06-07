// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
#define HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347


#include "iridium/enum.h"

#include "stream.h"


namespace iridium {
namespace io {


struct Event {
    DEFINE_CREATE(Event)
    DEFINE_ENUM(TOperation, OPEN, CLOSE, READ, WRITE, TIMEOUT, EOF_, ERROR_)
    DEFINE_ENUM(TStatus, BEGIN, END)
    
    Event(IStream::TSharedPtr const &stream, TOperation const &operation, TStatus const &status = TStatus::UNKNOWN);
    
    IStream::TSharedPtr stream;
    TOperation          operation;
    TStatus             status;
};


} // io
} // iridium


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
