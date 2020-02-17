// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
#define HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA


#include "iridium/parsing/serialization/node.h"
#include "iridium/io/buffer.h"

#include <string>
#include <chrono>


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace response {


DEFINE_ROOT_NODE_BEGIN(Http)
    static std::string const DEFAULT_SERVER_NAME;

    struct TMessageLine {
        std::string protocol;
        int         code;
        std::string reason;
    };
    DEFINE_ATTRIBUTE(TMessageLine, Message, TMessageLine( {"HTTP/1.1", 200, "OK"} ))
    DEFINE_NODE_BEGIN(Headers)
        struct THTTPDate {
            std::chrono::system_clock::time_point date;
        };
        DEFINE_ATTRIBUTE(THTTPDate, Date, THTTPDate())
        DEFINE_ATTRIBUTE(std::string, server, DEFAULT_SERVER_NAME)
        DEFINE_ATTRIBUTE(THTTPDate, LastModified, THTTPDate())
        DEFINE_ATTRIBUTE(size_t, ContentLength, 0)
        DEFINE_ATTRIBUTE(std::string, ContentType, "text/html")
//        DEFINE_ATTRIBUTE(std::string, Connection, "")
    DEFINE_NODE_END(Headers)
    DEFINE_ATTRIBUTE(Buffer, Body, Buffer())
DEFINE_ROOT_NODE_END()


} // response
} // http
} // protocol
} // io
} // iridium


DEFINE_CONVERT(iridium::io::protocol::http::response::THttp::TMessage, std::string)
DEFINE_CONVERT(std::string, iridium::io::protocol::http::response::THttp::TMessage)

DEFINE_CONVERT(iridium::io::protocol::http::response::THttp::THeaders::THTTPDate, std::string)
DEFINE_CONVERT(std::string, iridium::io::protocol::http::response::THttp::THeaders::THTTPDate)


#endif // HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
