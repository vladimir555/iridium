//#include "http.h"
//#include "socket.h"

//#include "utility/protocol/http/request.h"
//#include "utility/protocol/http/response.h"
//#include "utility/convertion/convert.h"
//#include "utility/logging/logger.h"

//#include "utility/parsing/implementation/parser_http.h"


//using utility::parsing::implementation::CHTTPParser;
//using utility::convertion::convert;
//using std::string;


//namespace {


////size_t const MIN_HTTP_HEADER_SIZE = 16;
//size_t const MIN_HTTP_HEADER_SIZE = 84;


//}


//namespace utility {
//namespace networking {
//namespace server {
//namespace implementation {


//CHTTP::CHTTP(URL const &url, IHTTP::THTTPHandlers const &http_handlers) {
//    ISocket::TSocketStreamsHandlers socket_handlers;

//// todo:
////    for (auto const &http_handler: http_handlers)
////        socket_handlers.push_back(CSocketHandler::create(http_handler));

//    m_socket = CSocket::create(url, socket_handlers);
//}


//void CHTTP::initialize() {
//    m_socket->initialize();
//}


//void CHTTP::finalize() {
//    m_socket->finalize();
//}


//CHTTP::CSocketHandler::CSocketHandler(IHTTPHandler::TSharedPtr const &http_handler)
//:
//    m_http_handler  (http_handler),
//    m_parser        (CHTTPParser::create())
//{}


//CHTTP::CSocketHandler::TSocketsEvents CHTTP::CSocketHandler::handle(TSocketsEvents const &sockets_events) {
//// todo:
//    for (auto const &socket_event: sockets_events) {
//        try {
//            SocketStreamEvent::TEvent::TEnumInternal e = socket_event->event;
//            switch (e) {
//            case SocketStreamEvent::TEvent::READ: {
//                auto packet = socket_event->socket->read();
//                socket_event->buffer_read.insert(socket_event->buffer_read.end(), packet.begin(), packet.end());
//                break;
//            }
//            case SocketStreamEvent::TEvent::WRITE: {
//                socket_event->socket->write(socket_event->buffer_write);
//                break;
//            }
//            default:
//                throw std::runtime_error("unknown state " + convert<string>(socket_event->event));
//            }
////            auto packet = socket_event->read();

////            if (packet.size() < MIN_HTTP_HEADER_SIZE) {
//////                LOGT << "skip socket " << socket->getURL();
////                sockets_events.push_back(socket_event);
////                continue; // <---
////            }

////            protocol::http::request::THttp    request      (m_parser->parse(convert<string>(packet)));
////            protocol::http::response::THttp   response    = m_http_handler->handle(request);
////            response.Headers.ContentLength      = response.Body.get().size();
////            packet                              = convert<ISocketStream::TPacket>(m_parser->compose(response.getNode()));
////            socket_event->write(packet);
//        } catch (std::exception const &e) {
////            LOGE << socket_event->getURL() << " " << e.what();
//        }
////        socket_event->close();
//    }
//    return TSocketsEvents(); // ----->
//}


//void CHTTP::CSocketHandler::initialize() {
//    LOGT << "start";
//}


//void CHTTP::CSocketHandler::finalize() {
//    LOGT << "stop";
//}


//} // implementation
//} // server
//} // networking
//} // utility
void dummy() {}
