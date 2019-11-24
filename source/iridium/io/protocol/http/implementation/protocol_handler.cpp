#include "protocol_handler.h"

#include "iridium/logging/logger.h"
#include "iridium/io/buffer.h"
#include "iridium/io/implementation/stream_reader_list.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/io/implementation/stream_splitter.h"
#include "iridium/io/implementation/transmitter.h"

#include <string>


using std::string;
using iridium::io::implementation::CStreamReaderList;
using iridium::io::implementation::CStreamReaderBuffer;
using iridium::io::implementation::CStreamWriterBuffer;
using iridium::io::implementation::CStreamSplitter;
using iridium::io::implementation::CTransmitter;


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


CProtocolHandler::CProtocolHandler() {
    LOGT << "create";
}


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


bool CProtocolHandler::update(
    ITransmitterStreams::TSharedPtr const &transmitter,
    Event::TSharedPtr               const &event)
{
    auto result = true;

    try {
        if (event->type == Event::TType::READ) {
            if (m_write_buffer) {
                LOGT << "write buffer: " << *m_write_buffer;
                auto header  = string(m_write_buffer->begin(), m_write_buffer->end());
                if  (header == "hello\r\nhello\r\n") {
                    LOGT << "parsed client request:\n" << *m_write_buffer;

                    auto stream_reader_list = CStreamReaderList::create();

                    stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(string("HELLO1\n"))));
                    stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(string("HELLO2\n"))));

                    transmitter->setReader(stream_reader_list);
                    transmitter->setWriter(event->stream);

                    event->type = Event::TType::WRITE;
                    m_write_buffer.reset();

                    LOGT << "begin write client";
                }
            } else {
                m_write_buffer = Buffer::create();
                transmitter->setReader(event->stream);
                transmitter->setWriter(CStreamWriterBuffer::create(m_write_buffer));

                LOGT << "begin read client";
            }
        }
    } catch (std::exception const &e) {
        LOGE << "http protocol error: " << e.what();
        result = false;
    }

    return result; // ----->


//        if (event->type == Event::TType::READ) {
//            m_write_buffer      = Buffer::create();
//            transmitter->setReader(event->stream);
//            transmitter->setWriter(CStreamWriterBuffer::create(m_write_buffer));

//            LOGT << "begin read client";
//            return true;
//        }

//        LOGT << "buffer = '" << string(m_write_buffer->begin(), m_write_buffer->end()) << "'";

//        if (event->type == Event::TType::READ &&
//            m_write_buffer &&
//            string(m_write_buffer->begin(), m_write_buffer->end()) == "hello\r\nhello\r\n")
//        {
//            LOGT << "parsed client request:\n" << *m_write_buffer;

//            auto stream_reader_list = CStreamReaderList::create();

//            stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(string("HELLO1\n"))));
//            stream_reader_list->add(CStreamReaderBuffer::create(Buffer::create(string("HELLO2\n"))));

//            transmitter->setReader(stream_reader_list);
//            transmitter->setWriter(event->stream);

//            //m_write_buffer->clear();

//            LOGT << "begin write client";
////            m_state++;
//            return true;
//        }

//        if (m_state  > 1 && event->type == Event::TType::CLOSE) {
//            m_state  = 0;
//            m_write_buffer->clear();
//            return false;
//        }
//    } catch (std::exception const &e) {
//        LOGE << "http protocol error: " << e.what();
//        return false; // ----->
//    }

//    return true; // ----->
}


} // implementation
} // http
} // protocol
} // io
} // iridium
