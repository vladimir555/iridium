/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream_handler.h"

#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/io/implementation/stream_splitter.h"
#include "iridium/io/implementation/stream_reader_list.h"
#include "iridium/io/implementation/transmitter.h"
#include "iridium/io/protocol/http/request.h"
#include "iridium/io/protocol/http/response.h"
#include "iridium/io/protocol/http/implementation/content_storage.h"
#include "iridium/parsing/implementation/parser_http.h"
#include "iridium/convertion/convert.h"

#include "iridium/logging/logger.h"

#include <string>


using iridium::io::implementation::CStreamSplitter;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::io::implementation::CStreamReaderBuffer;
using iridium::io::implementation::CStreamReaderList;
using iridium::io::implementation::CTransmitter;
using iridium::parsing::implementation::CNode;
using iridium::convertion::convert;

using std::string;


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


static size_t DEFAULT_HTTP_HEADER_SIZE_MIN = 79;
    

CStreamHandler::CStreamHandler(IStreamPort::TSharedPtr const &client_stream)
:
    m_client_stream(client_stream)
{}
    
    
Event::TType CStreamHandler::handle(Event::TType const &type) {
    if (!m_server_stream) {
        m_write_buffer  = Buffer::create();
        m_server_stream = CStreamSplitter::create( nullptr, CStreamWriterBuffer::create(m_write_buffer) );
        m_transmitter   = CTransmitter::create(m_client_stream, m_server_stream);
        
        LOGT << "begin read client";
    }

    if (m_write_buffer->size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
        string(m_write_buffer->end() - 4, m_write_buffer->end()) == "\r\n\r\n")
    {
        protocol::http::request ::THttp request (m_parser->parse(convert<string>(*m_write_buffer)));
        protocol::http::response::THttp response(CNode::create("http"));

        LOGT << "parsed client request:\n" << request.getNode();

        auto content = m_content_storage->getContent("." + request.Message.get().uri);

        response.Headers.ContentLength = response.Body.get().size() + content->getSize();

        auto stream_list    = CStreamReaderList::create();
        auto header_buffer  = Buffer::create(m_parser->compose(response.getNode()));

        LOGT << "header buffer:\n"  << *header_buffer;
        LOGT << "header uri: '"     << request.Message.get().uri << "'";

        stream_list->add(CStreamReaderBuffer::create(header_buffer));
        stream_list->add(content);

        m_server_stream->set(stream_list);
        m_transmitter = CTransmitter::create(m_server_stream, m_client_stream);
        
        LOGT << "begin write client";
    }

    try {
        
        if (m_transmitter->transmit())
            return type; // ----->
        else
            return Event::TType::CLOSE; // ----->
        
    } catch (std::exception const &e) {
        LOGE << e.what();
        return Event::TType::ERROR; // ----->
    }
    
    return Event::TType::CLOSE; // ----->

    
    //    for (auto event: events) {
    //        LOGT << "next";
    //        auto id     = event->stream->getID();
    //        auto peer   = m_peers->get(id);
    //
    //        LOGT << "event: id = " << id << " type " << event->type;
    //
    //        try {
    //            bool result = false;
    //            if (peer.client_stream) {
    //                if (peer.server_stream_writer) {
    //                    LOGT << "continue read client";
    //                    result = transmit(peer.client_stream, peer.server_stream_writer);
    //                    LOGT << 1;
    //
    //                    if (peer.buffer) {
    //                        LOGT << "client request buffer:\n'" << *peer.buffer << "'";
    //                    }
    //
    //                    if (result && peer.buffer &&
    //                        peer.buffer->size() > DEFAULT_HTTP_HEADER_SIZE_MIN &&
    //                        string(peer.buffer->end() - 4, peer.buffer->end()) == "\r\n\r\n")
    //                    {
    //                        protocol::http::request ::THttp request     (m_parser->parse(convert<string>(*peer.buffer)));
    //                        protocol::http::response::THttp response    (CNode::create("http"));
    //
    //                        LOGT << "parsed client request:\n" << request.getNode();
    //
    //                        auto content = m_content_storage->getContent("." + request.Message.get().uri);
    //
    //                        response.Headers.ContentLength = response.Body.get().size() + content->getSize();
    //
    //                        auto stream_list    = CStreamReaderList::create();
    //                        auto header_buffer  = Buffer::create(m_parser->compose(response.getNode()));
    //
    //                        LOGT << "header buffer:\n"  << *header_buffer;
    //                        LOGT << "header uri: '"     << request.Message.get().uri << "'";
    //
    //                        stream_list->add(CStreamReaderBuffer::create(header_buffer));
    //                        stream_list->add(content);
    //
    //                        peer.server_stream_writer.reset();
    //                        peer.server_stream_reader = stream_list;
    //
    //                        LOGT << "begin write client";
    //                    }
    //                }
    //
    //                if (peer.server_stream_reader) {
    //                    LOGT << "continue write client";
    //                    result = transmit(peer.server_stream_reader, peer.client_stream);
    //                    if (result)
    //                        events_.push_back(event);
    //                }
    //            } else {
    //                LOGT << "begin read";
    //                peer.buffer                 = Buffer::create();
    //                peer.client_stream          = std::dynamic_pointer_cast<ISocket>(event->stream);
    //                peer.server_stream_writer   = CStreamWriterBuffer::create(peer.buffer);
    //                result = true;
    //            }
    //
    //            if (result) {
    //                m_peers->set(id, peer);
    //            } else {
    //                LOGT << "close client";
    //                peer.client_stream->finalize();
    //                m_peers->del(id);
    //            }
    //        } catch (std::exception const &e) {
    //            LOGE << e.what() << ", close socket";
    //            peer.client_stream->finalize();
    //            m_peers->del(id);
    //        }
    //    }
}
    

} // implementation
} // http
} // protocol
} // io
} // iridium
