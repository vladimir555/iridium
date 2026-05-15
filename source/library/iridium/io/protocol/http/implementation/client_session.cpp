//#include "client_session.h"
//#include "iridium/io/protocol/http/request.h"
//#include "iridium/io/implementation/stream_buffer.h"
//#include "iridium/io/implementation/pipe.h"
//#include "iridium/io/net/implementation/socket_client.h"
//#include "iridium/parsing/implementation/http_parser.h"
//#include "iridium/assert.h"


//using iridium::io::implementation::CStreamReaderBuffer;
//using iridium::io::implementation::CStreamWriterBuffer;
//using iridium::io::implementation::CPipe;
//using iridium::io::net::implementation::CSocketClient;


//#include "iridium/threading/implementation/mutex.h"
//#include "iridium/threading/synchronized_scope.h"
//#include "iridium/logging/logger.h"
//namespace iridium {
//namespace io {
//namespace protocol {
//namespace http {
//namespace implementation {


//CClientSession::CClientSession(URI const &uri)
//:
//    m_pipe  (CPipe::create()),
//    m_uri   (uri)
//{}


//IStream::TSharedPtr CClientSession::getStream(IListener::TSharedPtr const &listener) {
//    if (m_socket)
//        return m_socket;
//    else
//        return m_socket = CSocketClient::create(m_uri, listener);
//}


//IPipe::TSharedPtr CClientSession::getPipe(IListener::Event::TConstSharedPtr const &event) {
//    LOGT << "event " << event->type << " fd " << event->stream->getID() << " count = " << count++;

//    if (event->stream == m_socket && event->type == IListener::Event::TType::OPEN) {
////        auto            parser = parsing::implementation::CHTTPParser::create();
////        request::THttp  http;

////        http.Headers.Host      = *assertExists(m_uri.getHost(), "uri does not contain hostname");
////        http.Headers.UserAgent = "iridium";
////        m_request_buffer       = Buffer::create(parser->compose(http.getNode()));
//        LOGT << "create request socket";

////        std::string request = ""
////            "GET / HTTP/1.1\r\n"
////            "Host: example.com:443\r\n"
////            "User-Agent: curi/7.58.0\r\n"
////            "Accept: */*"
////            "\r\n"
////            "\r\n";

//        std::string path = "/";
//        if (m_uri.getPath())
//            path = *m_uri.getPath();
//        std::string host = *m_uri.getHost();
//        std::string port = convertion::convert<std::string>(*m_uri.getPort());

//        std::string request = ""
//            "GET " + path + " HTTP/1.1\r\n"
//            "Host: " + host + ":" + port + "\r\n"
//            "User-Agent: curi/7.58.0\r\n"
//            "Accept: */*"
//            "\r\n"
//            "\r\n";


//        m_request_buffer = Buffer::create(request);
//        m_request_stream = CStreamReaderBuffer::create(m_request_buffer);
//        m_pipe->set(m_request_stream, m_socket);

//        return m_pipe; // ----->
//    }

//    if (event->stream == m_request_stream && event->type == IListener::Event::TType::EOF_) {
//        LOGT << "switch to getting response";

//        m_response_buffer = Buffer::create();
//        m_response_stream = CStreamWriterBuffer::create(m_response_buffer);
//        m_pipe->set(m_socket, m_response_stream);

//        return m_pipe; // ----->
//    }

//    if (m_response_buffer) {
//        LOGT << "response_buffer size = " << m_response_buffer->size();
//        LOGT << "response:" << *m_response_buffer;
//    }

//    if (m_response_buffer && m_response_buffer->size() > 900)
//        m_pipe.reset();

//    if (m_pipe)
//        LOGT << "return pipe";
//    else
//        LOGT << "return null";

//    return m_pipe;
//}


//} // implementation
//} // http
//} // protocol
//} // io
//} // iridium
