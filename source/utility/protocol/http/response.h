#ifndef HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
#define HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA


#include "utility/parsing/serialization/node.h"
#include "utility/networking/socket.h"

#include <string>
#include <chrono>


namespace utility {
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
            std::chrono::high_resolution_clock::time_point date;
        };
        DEFINE_ATTRIBUTE_DEFAULT(THTTPDate, Date, THTTPDate())
        DEFINE_ATTRIBUTE_DEFAULT(std::string, server, "utility")
        DEFINE_ATTRIBUTE_DEFAULT(THTTPDate, LastModified, THTTPDate())
        DEFINE_ATTRIBUTE_DEFAULT(int, ContentLength, 0)
        DEFINE_ATTRIBUTE_DEFAULT(std::string, ContentType, "text/html")
//        DEFINE_ATTRIBUTE_DEFAULT(std::string, Connection, "")
    DEFINE_NODE_END(Headers)
    DEFINE_ATTRIBUTE_DEFAULT(networking::ISocket::TPacket, Body, networking::ISocket::TPacket())
DEFINE_ROOT_NODE_END()


} // response
} // http
} // protocol
} // utility


DEFINE_CONVERT(utility::protocol::http::response::THttp::TMessage, std::string)
DEFINE_CONVERT(std::string, utility::protocol::http::response::THttp::TMessage)

DEFINE_CONVERT(utility::protocol::http::response::THttp::THeaders::THTTPDate, std::string)
DEFINE_CONVERT(std::string, utility::protocol::http::response::THttp::THeaders::THTTPDate)


#endif // HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
