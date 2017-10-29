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


//size_t const MIN_HTTP_HEADER_SIZE = 16;
size_t const MIN_HTTP_HEADER_SIZE = 93;


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


CHTTP::CSocketHandler::TSockets CHTTP::CSocketHandler::handleItems(TSockets const &sockets_) {
    TSockets sockets;

    for (auto const &socket: sockets_) {
        try {
//            ISocketStream::map_url_read_cache_mutex.lock();
//            auto packet = ISocketStream::map_url_read_cache.at(convert<string>(socket->getURL()));
//            auto p      = socket->read();
//            packet.insert(packet.end(), p.begin(), p.end());
//            ISocketStream::map_url_read_cache[convert<string>(socket->getURL())] = packet;
//            ISocketStream::map_url_read_cache_mutex.unlock();
            auto packet      = socket->read();

            // todo: map url port packet
            if (packet.size() < MIN_HTTP_HEADER_SIZE) {
//                LOGT << "skip socket " << socket->getURL();
                sockets.push_back(socket);
                continue; // <---
            }

//            LOGT << "url " << socket->getURL() << " read size " << packet.size();

            http::request::THttp    request      (m_parser->parse(convert<string>(packet)));
            http::response::THttp   response    = m_http_handler->handle(request);

            response.Headers.ContentLength      = response.Body.get().size();
            packet                              = convert<ISocketStream::TPacket>(m_parser->compose(response.getNode()));

//            LOGT << x"write:\n" << packet;

            socket->write(packet);
        } catch (std::exception const &e) {
            LOGE << "url " << socket->getURL() << " handler error: " << e.what();
        }
        socket->close();
//        ISocketStream::map_url_read_cache_mutex.lock();
//        LOGT << "delete url " << socket->getURL();
//        ISocketStream::map_url_read_cache.erase(convert<string>(socket->getURL()));
//        ISocketStream::map_url_read_cache_mutex.unlock();
    }

    return sockets; // ----->
}


void CHTTP::CSocketHandler::handleStart() {
    LOGT << "start";
}


void CHTTP::CSocketHandler::handleStop() {
    LOGT << "stop";
}


} // implementation
} // server
} // networking
} // utility
