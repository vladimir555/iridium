#include "protocol_handler.h"

#include "iridium/logging/logger.h"
#include "iridium/io/buffer.h"
#include "iridium/io/implementation/stream_reader_list.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/io/implementation/stream_splitter.h"
#include "iridium/io/implementation/transmitter.h"

#include "iridium/convertion/convert.h"

#include "iridium/io/protocol/http/request.h"
#include "iridium/io/protocol/http/response.h"

#include "content_storage.h"

#include "iridium/parsing/implementation/parser_http.h"
#include "iridium/parsing/implementation/node.h"

#include <string>


using std::string;
using iridium::io::implementation::CStreamReaderList;
using iridium::io::implementation::CStreamReaderBuffer;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::io::implementation::CStreamSplitter;
using iridium::io::implementation::CTransmitter;
using iridium::convertion::convert;
using iridium::parsing::implementation::CNode;
using iridium::parsing::implementation::CHTTPParser;


static size_t const DEFAULT_HTTP_HEADER_SIZE_MIN    = 78;
static string const DEFAULT_HTTP_PATH               = "/index.html";


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


CProtocolHandler::CProtocolHandler() {
    LOGT << "create";
    // todo:
    m_parser            = CHTTPParser::create();
    // todo: path, path check
    m_content_storage   = CContentStorage::create(".");
}


bool CProtocolHandler::update(
    ITransmitterStreams::TSharedPtr const &transmitter,
    Event::TSharedPtr               const &event)
{
    auto result = true;

    // todo:
    // http codes enum;
    // http date fix;
    // content_stream initizlize / finalize fix;
    // rm debug sleeps, logs
    // uri path checking

    try {
        if (event->type == Event::TType::READ) {
            if (m_peer_buffer) {
                LOGT << "write buffer:\n" << *m_peer_buffer << "\nsize = " << m_peer_buffer->size();
                auto header  = string(m_peer_buffer->begin(), m_peer_buffer->end());

                if (m_peer_buffer->size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
                    string(m_peer_buffer->end() - 4, m_peer_buffer->end()) == "\r\n\r\n")
                {
                    request ::THttp request     (m_parser->parse(convert<string>(*m_peer_buffer)));
                    response::THttp response    (CNode::create("http"));

                    LOGT << "parsed client request:" << request.getNode();

                    auto uri = request.Message.get().uri;
                    if (uri == "/")
                        uri  = DEFAULT_HTTP_PATH;

                    fs::IFileStreamReader::TSharedPtr content_stream_reader;
                    try {
                        content_stream_reader = m_content_storage->getContent(uri);
                        if (content_stream_reader) {
                            content_stream_reader->initialize();
                            response.Headers.ContentLength = response.Body.get().size() + content_stream_reader->getSize();
                            response.Message = {"HTTP/1.1", 200, "OK"};
                        } else {
                            response.Message = {"HTTP/1.1", 404, "Error"};
                        }
                        response.Headers.Date.set({std::chrono::system_clock::now()});
                    } catch (std::exception const &e) {
                        LOGE << "http handle error: " << e.what();
                        if (content_stream_reader)
                            content_stream_reader->finalize();
                        content_stream_reader.reset();
                        response.Message = {"HTTP/1.1", 500, "Internal Server Error"};
                    }

                    auto stream_reader_list = CStreamReaderList::create();
                    stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(m_parser->compose(response.getNode()))));

                    if (content_stream_reader)
                        stream_reader_list->add(content_stream_reader);

                    transmitter->setReader(stream_reader_list);
                    transmitter->setWriter(event->stream);

                    event->type = Event::TType::WRITE;
                    m_peer_buffer.reset();

                    LOGT << "begin write to client";
                }
            } else {
                m_peer_buffer = Buffer::create();
                transmitter->setReader(event->stream);
                transmitter->setWriter(CStreamWriterBuffer::create(m_peer_buffer));

                LOGT << "begin read client";
            }
        }
        if (event->type == Event::TType::CLOSE)
            result = false;
    } catch (std::exception const &e) {
        LOGE << "http protocol error: " << e.what();
        result = false;
    }

    return result; // ----->
}


} // implementation
} // http
} // protocol
} // io
} // iridium
