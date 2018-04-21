#include "protocol.h"

#include "utility/protocol/http/request.h"
#include "utility/protocol/http/response.h"
#include "utility/protocol/implementation/packet.h"
#include "utility/parsing/implementation/parser_http.h"

#include "uri_fs_mapper.h"

#include "utility/logging/logger.h"

using utility::convertion::convert;
using utility::protocol::implementation::CPacket;
using utility::parsing::implementation::CHTTPParser;
using std::string;


namespace utility {
namespace protocol {
namespace http {
namespace implementation {


CProtocol::CProtocol(IHTTPHandler::TSharedPtr const &http_handler)
:
    m_parser        (CHTTPParser::create()),
    m_http_handler  (http_handler)
{
    if (!m_http_handler)
        m_http_handler = CURIFSMapper::create("./");
}


IPacket::TSharedPtr CProtocol::exchange(IPacket::TSharedPtr const &request_) {
    if (request_->getData().size() < 79)
        return nullptr;

    request::THttp  request (m_parser->parse(convert<string>(request_->getData())));
    response::THttp response(m_http_handler->handle(request));

    response.Headers.ContentLength = response.Body.get().size();

    return CPacket::create(convert<IPacket::TData>(m_parser->compose(response.getNode()))); // ----->
}


} // implementation
} // http
} // protocol
} // utility
