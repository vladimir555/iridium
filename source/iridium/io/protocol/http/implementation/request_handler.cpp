#include "request_handler.h"
#include "iridium/io/protocol/http/request.h"
#include "iridium/io/implementation/stream_buffer.h"
#include "iridium/parsing/implementation/parser_http.h"
#include "iridium/assert.h"


using iridium::io::implementation::CStreamReaderBuffer;


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


RequestHandler::RequestHandler()
:
    m_url               ("https://example.com"),
    m_is_request_state  (true)
{}


bool RequestHandler::redirectStreams(
    ITransmitterStreams::TSharedPtr const &transmitter,
    Event::TSharedPtr               const &event)
{
    if (m_is_request_state) {
        if (m_request_buffer) {

        } else {
            parsing::IParser::TSharedPtr parser = parsing::implementation::CHTTPParser::create();
            request::THttp http;
            http.Headers.Host       = *assertExists(m_url.getHost(), "url is not contained host name");
            http.Headers.UserAgent  = "iridium";

            m_request_buffer        = Buffer::create(parser->compose(http.getNode()));
            transmitter->set(
                CStreamReaderBuffer::create(m_request_buffer),
                std::dynamic_pointer_cast<IStreamWriter>(event->stream));
            return true;
        }
    }

    return true;
}


} // implementation
} // http
} // protocol
} // io
} // iridium
