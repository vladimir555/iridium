#ifndef HEADER_STREAM_CONTEXT_427EA98F_8CA9_445D_BBE3_A95D836E961E
#define HEADER_STREAM_CONTEXT_427EA98F_8CA9_445D_BBE3_A95D836E961E


#include "utility/io/stream.h"
#include "utility/io/stream_proxy.h"
#include "utility/io/listener.h"
#include "utility/io/protocol/protocol.h"


namespace utility {
namespace io {


class StreamContext {
public:
    DEFINE_CREATE(StreamContext)

    StreamContext(IStream::TSharedPtr const &stream, protocol::IProtocol::TSharedPtr const protocol);

    void update(IListener::IListener::Event::TEvent const &event);

private:
    protocol::IProtocol::TSharedPtr m_protocol;
    IStream::TSharedPtr             m_stream;
    IStreamProxy::TSharedPtr        m_stream_proxy;
};


} // io
} // utility


#endif // HEADER_STREAM_CONTEXT_427EA98F_8CA9_445D_BBE3_A95D836E961E
