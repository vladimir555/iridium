// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77
#define HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77


#include "iridium/enum.h"

#include <string>


namespace iridium {
namespace io {
namespace protocol {
namespace http {


DEFINE_ENUM(
    TMethod,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
)


} // http
} // protocol
} // io
} // iridium



#endif // HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77
