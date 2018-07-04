#include "protocol.h"

#include "utility/io/protocol/http/request.h"
#include "utility/io/protocol/http/response.h"
#include "utility/io/protocol/implementation/packet.h"
#include "utility/parsing/implementation/parser_http.h"

#include "uri_fs_mapper.h"


using utility::convertion::convert;
using utility::io::protocol::implementation::CPacket;
using utility::parsing::implementation::CHTTPParser;
using std::string;


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


CProtocol::CProtocol(IHTTPSource::TSharedPtr const &http_handler)
:
    m_parser        (CHTTPParser::create()),
    m_http_handler  (http_handler)
{
    if (!m_http_handler)
        m_http_handler = CURIFSMapper::create("./");
}


//IPacket::TSharedPtr CProtocol::exchange(IPacket::TSharedPtr const &request_) {
//    if (request_->getData().size() < 79)
//        return nullptr;

//    request::THttp  request (m_parser->parse(convert<string>(request_->getData())));
//    response::THttp response(m_http_handler->handle(request));

//    response.Headers.ContentLength = response.Body.get().size();

//    return CPacket::create(convert<IPacket::TData>(m_parser->compose(response.getNode()))); // ----->
//}


} // implementation
} // http
} // protocol
} // io
} // utility
