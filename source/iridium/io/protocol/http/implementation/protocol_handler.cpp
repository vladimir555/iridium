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


static size_t const DEFAULT_HTTP_HEADER_SIZE_MIN = 78;


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

    try {
        if (event->type == Event::TType::READ) {
            if (m_write_buffer) {
                LOGT << "write buffer:\n" << *m_write_buffer << "\nsize = " << m_write_buffer->size();
                auto header  = string(m_write_buffer->begin(), m_write_buffer->end());

                if (m_write_buffer->size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
                    string(m_write_buffer->end() - 4, m_write_buffer->end()) == "\r\n\r\n")
                {
                    request ::THttp request     (m_parser->parse(convert<string>(*m_write_buffer)));
                    response::THttp response    (CNode::create("http"));

                    LOGT << "parsed client request:\n" << request.getNode();

                    auto content_stream_reader      = m_content_storage->getContent(request.Message.get().uri);
                    LOGT << "response.Body.get().size()         = " << response.Body.get().size();
                    LOGT << "content_stream_reader->getSize()   = " << content_stream_reader->getSize();
                    response.Headers.ContentLength  = response.Body.get().size() + content_stream_reader->getSize();

                    auto stream_reader_list = CStreamReaderList::create();
                    auto header_buffer      = Buffer::create(m_parser->compose(response.getNode()));

                    stream_reader_list->add(CStreamReaderBuffer::create(header_buffer));
                    stream_reader_list->add(content_stream_reader);

                    transmitter->setReader(stream_reader_list);
                    transmitter->setWriter(event->stream);

                    event->type = Event::TType::WRITE;
                    m_write_buffer.reset();

                    LOGT << "begin write to client";
                }
            } else {
                m_write_buffer = Buffer::create();
                transmitter->setReader(event->stream);
                transmitter->setWriter(CStreamWriterBuffer::create(m_write_buffer));

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
