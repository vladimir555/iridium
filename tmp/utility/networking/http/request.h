#ifndef HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
#define HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6


#include  "utility/parsing/serialization/node.h"
#include "utility/enum.h"

#include <string>

#include "method.h"


namespace utility {
namespace networking {
namespace http {
namespace request {


DEFINE_ROOT_NODE_BEGIN(Http)
    struct TMessageLine {
        TMethod     method;
        std::string uri;
        std::string protocol;
    };
    DEFINE_ATTRIBUTE(TMessageLine, Message)
    DEFINE_NODE_BEGIN(Headers)
        DEFINE_ATTRIBUTE(std::string, Host)
        DEFINE_ATTRIBUTE(std::string, UserAgent)
        DEFINE_ATTRIBUTE_DEFAULT(std::string, Connection, "")
        DEFINE_ATTRIBUTE_LIST(std::string, Accept)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptLanguage)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptEncoding)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptCharset)
        DEFINE_ATTRIBUTE_LIST(std::string, Cookie)
    DEFINE_NODE_END(Headers)
    DEFINE_ATTRIBUTE(std::string, Body)
DEFINE_ROOT_NODE_END()


} // request
} // http
} // networking
} // utility


DEFINE_CONVERT(utility::networking::http::request::THttp::TMessageLine, std::string)
DEFINE_CONVERT(std::string, utility::networking::http::request::THttp::TMessageLine)


#endif // HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
