// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_F84E6917_BBFF_4221_8C01_335F46089357
#define HEADER_EVENT_F84E6917_BBFF_4221_8C01_335F46089357


#include "context.h"


namespace iridium {
namespace io {


struct TEvent {
    DEFINE_CREATE(TEvent)
    DEFINE_ENUM(
        TOperation,
        OPEN,
        READ,
        READ_END,
        WRITE,
        WRITE_END,
        TIMEOUT,
        ERROR_,
        CLOSE);

    TOperation
        operation;
    URI::TSharedPtr
        uri;
    IContext::TSharedPtr
        context;
};


} // io
} // iridium


DEFINE_ENUM_CONVERT(iridium::io::TEvent::TOperation);
DEFINE_CONVERT(std::string, iridium::io::TEvent);


#endif // HEADER_EVENT_F84E6917_BBFF_4221_8C01_335F46089357
