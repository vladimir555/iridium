// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SESSION_6E49C5FE_ACE3_453D_B03A_222083C6A96C
#define HEADER_SESSION_6E49C5FE_ACE3_453D_B03A_222083C6A96C


#include "iridium/io/pipe.h"
#include "iridium/io/url.h"


namespace iridium {
namespace io {
namespace protocol {


class ISession {
public:
    DEFINE_INTERFACE(ISession)
    virtual IStream::TSharedPtr getStream(IListener::TSharedPtr const &listener) = 0;
    virtual IPipe::TSharedPtr   getPipe(IListener::Event::TConstSharedPtr const &event) = 0;
};


} // protocol
} // io
} // iridium


#endif // HEADER_SESSION_6E49C5FE_ACE3_453D_B03A_222083C6A96C
