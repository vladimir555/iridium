#ifndef HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
#define HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA


#include "utility/parsing/serialization/node.h"
#include "utility/io/net/types.h"

#include <string>
#include <chrono>


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace response {


DEFINE_ROOT_NODE_BEGIN(Http)
    struct TMessageLine {
        std::string protocol;
        int code;
        std::string reason;
    };
    DEFINE_ATTRIBUTE_DEFAULT(TMessageLine, Message, TMessageLine( {"HTTP/1.1", 200, "OK"} ))
    DEFINE_NODE_BEGIN(Headers)
        struct THTTPDate {
            std::chrono::system_clock::time_point date;
        };
        DEFINE_ATTRIBUTE_DEFAULT(THTTPDate, Date, THTTPDate())
        DEFINE_ATTRIBUTE_DEFAULT(std::string, server, "utility")
        DEFINE_ATTRIBUTE_DEFAULT(THTTPDate, LastModified, THTTPDate())
        DEFINE_ATTRIBUTE_DEFAULT(size_t, ContentLength, 0)
        DEFINE_ATTRIBUTE_DEFAULT(std::string, ContentType, "text/html")
//        DEFINE_ATTRIBUTE_DEFAULT(std::string, Connection, "")
    DEFINE_NODE_END(Headers)
    DEFINE_ATTRIBUTE_DEFAULT(io::net::TPacket, Body, io::net::TPacket())
DEFINE_ROOT_NODE_END()


} // response
} // http
} // protocol
} // io
} // utility


DEFINE_CONVERT(utility::io::protocol::http::response::THttp::TMessage, std::string)
DEFINE_CONVERT(std::string, utility::io::protocol::http::response::THttp::TMessage)

DEFINE_CONVERT(utility::io::protocol::http::response::THttp::THeaders::THTTPDate, std::string)
DEFINE_CONVERT(std::string, utility::io::protocol::http::response::THttp::THeaders::THTTPDate)


#endif // HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
