#include "http.h"
#include "socket.h"

#include "utility/protocol/http/request.h"
#include "utility/protocol/http/response.h"
#include "utility/convertion/convert.h"
#include "utility/logging/logger.h"

#include "utility/parsing/implementation/parser_http.h"


using utility::parsing::implementation::CHTTPParser;
using utility::convertion::convert;
using std::string;


namespace {


//size_t const MIN_HTTP_HEADER_SIZE = 16;
size_t const MIN_HTTP_HEADER_SIZE = 84;


}


namespace utility {
namespace networking {
namespace server {
namespace implementation {


CHTTP::CHTTP(URL const &url, IHTTP::THTTPHandlers const &http_handlers) {
    ISocket::TSocketStreamsHandlers socket_handlers;

    for (auto const &http_handler: http_handlers)
        socket_handlers.push_back(CSocketHandler::create(http_handler));

    m_socket = CSocket::create(url, socket_handlers);
}


void CHTTP::initialize() {
    m_socket->initialize();
}


void CHTTP::finalize() {
    m_socket->finalize();
}


CHTTP::CSocketHandler::CSocketHandler(IHTTPHandler::TSharedPtr const &http_handler)
:
    m_http_handler  (http_handler),
    m_parser        (CHTTPParser::create())
{}


CHTTP::CSocketHandler::TSockets CHTTP::CSocketHandler::handle(TSockets const &sockets_) {
    TSockets sockets;
    for (auto const &socket: sockets_) {
        try {
            auto packet      = socket->read();

            if (packet.size() < MIN_HTTP_HEADER_SIZE) {
//                LOGT << "skip socket " << socket->getURL();
                sockets.push_back(socket);
                continue; // <---
            }

            protocol::http::request::THttp    request      (m_parser->parse(convert<string>(packet)));
            protocol::http::response::THttp   response    = m_http_handler->handle(request);
            response.Headers.ContentLength      = response.Body.get().size();
            packet                              = convert<ISocketStream::TPacket>(m_parser->compose(response.getNode()));
            socket->write(packet);
        } catch (std::exception const &e) {
            LOGE << socket->getURL() << " " << e.what();
        }
        socket->close();
    }
    return sockets; // ----->
}


void CHTTP::CSocketHandler::initialize() {
    LOGT << "start";
}


void CHTTP::CSocketHandler::finalize() {
    LOGT << "stop";
}


} // implementation
} // server
} // networking
} // utility
