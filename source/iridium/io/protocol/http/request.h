// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
#define HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6


#include  "iridium/parsing/serialization/node.h"
#include "iridium/enum.h"

#include <string>

#include "method.h"


namespace iridium::io::protocol::http::request {


DEFINE_ROOT_NODE_BEGIN(Http)
    struct TMessageLine {
        TMethod     method;
        std::string uri;
        std::string protocol;
    };
    DEFINE_ATTRIBUTE(TMessageLine, Message, TMessageLine({TMethod::GET, "", ""}))
    DEFINE_NODE_BEGIN(Headers)
        DEFINE_ATTRIBUTE(std::string, Host, "")
        DEFINE_ATTRIBUTE(std::string, UserAgent, "")
        DEFINE_ATTRIBUTE(std::string, Connection, "")
        DEFINE_ATTRIBUTE_LIST(std::string, Accept)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptLanguage)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptEncoding)
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptCharset)
        DEFINE_ATTRIBUTE_LIST(std::string, Cookie)
    // todo: Keep-Alive: timeout=5, max=1000
    DEFINE_NODE_END(Headers)
    DEFINE_ATTRIBUTE(std::string, Body, "")
DEFINE_ROOT_NODE_END()


} // iridium::io::protocol::http::request


DEFINE_CONVERT(std::string, iridium::io::protocol::http::request::THttp::TMessageLine)
DEFINE_CONVERT(iridium::io::protocol::http::request::THttp::TMessageLine, std::string)


#endif // HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
