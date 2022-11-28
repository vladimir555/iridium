//#include "peer_session.h"

//#include "iridium/logging/logger.h"

//#include "iridium/io/protocol/http/request.h"
//#include "iridium/io/protocol/http/response.h"
//#include "iridium/io/implementation/stream_reader_list.h"
//#include "iridium/io/implementation/stream_buffer.h"
//#include "iridium/io/implementation/stream_splitter.h"
//#include "iridium/io/implementation/pipe.h"

//#include "iridium/parsing/implementation/parser_http.h"
//#include "iridium/parsing/implementation/node.h"

////#include "iridium/convertion/convert.h"

//#include "content_storage.h"
//#include "mime.h"

//#include "iridium/items.h"

////#include <string>


//using std::string;
//using iridium::io::implementation::CStreamReaderList;
//using iridium::io::implementation::CStreamReaderBuffer;
//using iridium::io::implementation::CStreamWriterBuffer;
//using iridium::io::implementation::CPipe;
//using iridium::convertion::convert;
//using iridium::parsing::implementation::CNode;
//using iridium::parsing::implementation::CHTTPParser;


//static size_t   const DEFAULT_HTTP_HEADER_SIZE_MIN  = 78;
//static char     const DEFAULT_HTTP_PATH[]           = "/index.html";


//namespace iridium {
//namespace io {
//namespace protocol {
//namespace http {
//namespace implementation {


//CPeerSession::CPeerSession(IStreamPort::TSharedPtr const &peer)
//:
//    m_peer              (peer),
//    m_pipe              (CPipe::create()),
//    m_parser            (CHTTPParser::create()),
//    m_is_keep_alive     (false),
//    m_content_storage   (CContentStorage::create(""))
//{}


//IStream::TSharedPtr CPeerSession::getStream(IListener::TSharedPtr const &/*listener*/) {
//    return m_peer; // ----->
//}


//IPipe::TSharedPtr CPeerSession::getPipe(IListener::Event::TConstSharedPtr const &event) {
//    // todo:
//    // http codes enum;
//    // +http date fix;
//    // +content_stream initizlize / finalize fix;
//    // rm debug sleeps, logs
//    // +uri path checking
//    // http timeout
//    // buffer size fix

//    LOGT << "event " << event->type << " fd " << event->stream->getID();

//    typedef IListener::Event::TType TEventType;

//    try {
//        if (event->stream == m_peer) {
//            if (event->type == TEventType::OPEN) {
//                m_request_buffer = Buffer::create();
//                m_pipe->set(m_peer, CStreamWriterBuffer::create(m_request_buffer));
//            }

//            if (checkOneOf(event->type, TEventType::READ, TEventType::EOF_) && m_pipe->getReader() == m_peer) {
//                auto header = string(m_request_buffer->begin(), m_request_buffer->end());

//                // finished reading http header
//                if (header.size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
//                    header.substr(header.size() - 4) == "\r\n\r\n")
//                {
//                    request ::THttp request (m_parser->parse(header));
//                    response::THttp response(CNode::create("http"));

//                    auto uri = request.Message.get().uri;
//                    if (uri == "/")
//                        uri  = DEFAULT_HTTP_PATH;

//                    m_is_keep_alive = request.Headers.Connection.get() == "Keep-Alive";

//                    fs::IFileStreamReader::TSharedPtr content_stream_reader;
//                    try {
//                        content_stream_reader = m_content_storage->getContent(uri);
//                        if (content_stream_reader) {
//                            content_stream_reader->initialize();
//                            response.Headers.ContentLength  = response.Body.get().size() + content_stream_reader->getSize();
//                            response.Message                = {"HTTP/1.1", 200, "OK"};
//                            response.Headers.LastModified   = {content_stream_reader->getStatus().last_modified};
//                            // todo: optimize
//                            response.Headers.ContentType    = MIME::instance().getByFileNameExtension(split(uri, ".").back());
//                        } else {
//                            response.Message                = {"HTTP/1.1", 404, "Error"};
//                        }
//                        response.Headers.Date               = {std::chrono::system_clock::now()};

//                    } catch (std::exception const &e) {
//                        LOGE << "http handle error: " << e.what();
//                        if (content_stream_reader)
//                            content_stream_reader->finalize();
//                        content_stream_reader.reset();
//                        response.Message = {"HTTP/1.1", 500, "Internal Server Error"};
//                    }

//                    auto stream_reader_list = CStreamReaderList::create();
//                    stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(m_parser->compose(response.getNode()))));

//                    if (content_stream_reader)
//                        stream_reader_list->add(content_stream_reader);

//                    m_pipe->set(stream_reader_list, m_peer);
//                } else {
//                    // wrong header, close peer
//                    if (event->type == TEventType::EOF_)
//                        return nullptr; // ----->
//                }
//            }

//            // finished writing
//            if (event->type == TEventType::EOF_ && m_pipe->getWriter() == m_peer) {
//                if (m_is_keep_alive) {
//                    m_request_buffer->clear();
//                    m_pipe->set(m_peer, CStreamWriterBuffer::create(m_request_buffer));
//                } else
//                    return nullptr; // ----->
//            }
//        }
//    } catch (std::exception const &e) {
//        LOGE << "peer error: " << e.what();
//        return nullptr; // ----->
//    }

//    return m_pipe; // ----->
//}


//} // implementation
//} // http
//} // protocol
//} // io
//} // iridium
