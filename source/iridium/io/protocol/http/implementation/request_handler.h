#ifndef HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A
#define HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A


#include "iridium/io/protocol/protocol_factory.h"
#include "iridium/io/net/url.h"
#include "iridium/io/buffer.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class RequestHandler: public IProtocolHandler {
public:
    DEFINE_IMPLEMENTATION(RequestHandler)
    RequestHandler();
    bool redirectStreams(ITransmitterStreams::TSharedPtr const &transmitter, Event::TSharedPtr const &event) override;
private:
    net::URL  m_url;
    bool m_is_request_state;
    Buffer::TSharedPtr m_request_buffer;
    Buffer::TSharedPtr m_response_buffer;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A
