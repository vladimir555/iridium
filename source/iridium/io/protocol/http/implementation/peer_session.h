#ifndef HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4
#define HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4


#include "iridium/parsing/parser.h"
#include "iridium/io/stream_splitter.h"
#include "iridium/io/protocol/session.h"
#include "iridium/io/protocol/http/content_storage.h"


#include "iridium/io/protocol/session.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CPeerSession: public ISession {
public:
    DEFINE_IMPLEMENTATION(CPeerSession)
    CPeerSession(IStreamPort::TSharedPtr const &peer);

    IStream::TSharedPtr getStream(IListener::TSharedPtr const &listener) override;
    IPipe::TSharedPtr   getPipe  (IListener::Event::TConstSharedPtr const &event) override;

private:
    IStreamPort::TSharedPtr         m_peer;
    Buffer::TSharedPtr              m_request_buffer;
//    IStreamSplitter::TSharedPtr     m_server_stream;
    IPipe::TSharedPtr               m_pipe;
    parsing::IParser::TSharedPtr    m_parser;
    bool                            m_is_keep_alive;
    IContentStorage::TSharedPtr     m_content_storage;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_HANDLER_AC6719DC_DDB4_4F4A_ACF1_A4EA7453E5B4
