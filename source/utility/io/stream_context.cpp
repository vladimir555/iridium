#include "stream_context.h"



using utility::convertion::convert;
using utility::io::protocol::IProtocol;
using std::string;


namespace utility {
namespace io {


StreamContext::StreamContext(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const protocol)
:
    m_protocol  (protocol),
    m_stream    (stream)
{}


void StreamContext::update(IListener::Event::TEvent const &event) {
    m_stream_proxy = m_protocol->update(m_stream_proxy, m_stream);
    m_stream_proxy->transmit();
}


} // io
} // utility
