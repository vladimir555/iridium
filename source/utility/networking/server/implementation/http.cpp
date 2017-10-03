#include "http.h"
#include "socket.h"

#include "utility/networking/http/request.h"
#include "utility/networking/http/response.h"
#include "utility/logging/logger.h"
#include "utility/parsing/implementation/parser_http.h"
#include "utility/convertion/convert.h"


using utility::parsing::implementation::CHTTPParser;
using utility::convertion::convert;
using std::string;


namespace {


size_t const MIN_HTTP_HEADER_SIZE = 16;


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


CHTTP::CSocketHandler::TSockets CHTTP::CSocketHandler::handleItems(TSockets const &sockets) {
    TSockets sockets_;

    for (auto const &socket: sockets) {
        try {
            auto packet = socket->read();

            if (packet.size() < MIN_HTTP_HEADER_SIZE) {
                sockets_.push_back(socket);
                continue; // <---
            }

            // drops slow connections with not full headers
            http::request::THttp    http_request(m_parser->parse(convert<string>(packet)));
            IHTTPHandler::TRequest  request;
            http::response::THttp   response;

            request.uri                     = http_request.Message.get().uri;
            auto                    body    = m_http_handler->handle(request);
            response.Body                   = body;
            response.Headers.ContentLength  = body.size();
            response.Headers.server         = "utility";
            response.Headers.Connection     = "Closed";
            response.Message.set( {"HTTP/1.1", 200, "OK"} );
            packet                          = convert<ISocketStream::TPacket>(m_parser->compose(response.getNode()));

            socket->write(packet);
        } catch (std::exception const &e) {
            LOGE << e.what();
            socket->close();
        }
    }

    return sockets_;
}


void CHTTP::CSocketHandler::handleStart() {
}


void CHTTP::CSocketHandler::handleStop() {
}


} // implementation
} // server
} // networking
} // utility
