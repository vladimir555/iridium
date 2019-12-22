#ifndef HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4
#define HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4


#include "iridium/parsing/parser.h"
#include "iridium/io/stream_splitter.h"
#include "iridium/io/protocol/protocol_factory.h"
#include "iridium/io/protocol/http/content_storage.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CProtocolHandler: public IProtocolHandler {
public:
    DEFINE_IMPLEMENTATION(CProtocolHandler)
//    CProtocolHandler() = default;
    CProtocolHandler();
    bool update(
        ITransmitterStreams::TSharedPtr const &transmitter,
        Event::TSharedPtr               const &event) override;

private:
    IStreamSplitter::TSharedPtr     m_server_stream;
    IStreamPort::TSharedPtr         m_client_stream;
    Buffer::TSharedPtr              m_peer_buffer;

    parsing::IParser::TSharedPtr    m_parser;
    IContentStorage::TSharedPtr     m_content_storage;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4
