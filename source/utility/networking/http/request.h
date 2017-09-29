#ifndef HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
#define HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6


#include  "utility/parsing/serialization/node.h"
#include "utility/enum.h"

#include <string>

#include "method.h"


namespace utility {
namespace networking {
namespace http {


DEFINE_ENUM(
    TAllow,
    GET,
    OPTIONS,
    HEAD
)


DEFINE_ROOT_NODE_BEGIN(Http)
    struct TRequestLine {
        TMethod     method;
        std::string uri;
        std::string protocol;
    };
    DEFINE_ATTRIBUTE(TRequestLine, Message)
    DEFINE_NODE_BEGIN(Headers)
        DEFINE_ATTRIBUTE(std::string, Host)
        DEFINE_ATTRIBUTE(std::string, UserAgent)
        DEFINE_ATTRIBUTE(int, KeepAlive)
        DEFINE_ATTRIBUTE(std::string, Connection)
        DEFINE_ATTRIBUTE_LIST(std::string, Accept)
    DEFINE_NODE_END(Headers)
DEFINE_ROOT_NODE_END()


} // http
} // networking
} // utility


DEFINE_CONVERT(utility::networking::http::THttp::TRequestLine, std::string)
DEFINE_CONVERT(std::string, utility::networking::http::THttp::TRequestLine)


#endif // HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
