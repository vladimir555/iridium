#include "stream_proxy.h"

#include "utility/convertion/convert.h"


using std::string;
using utility::convertion::convert;


namespace utility {
namespace io {
namespace implementation {


CStreamProxy::CStreamProxy(
    IStreamReader::TSharedPtr   const &stream_reader,
    IStreamWriter::TSharedPtr   const &stream_writer,
    size_t                      const &buffer_transmitting_size)
:
    m_reader                    (stream_reader),
    m_writer                    (stream_writer),
    m_buffer_transmitting_size  (buffer_transmitting_size)
{}


void CStreamProxy::transmit() {
    auto buffer         = m_reader->read(m_buffer_transmitting_size);
    auto written_size   = m_writer->write(buffer);

    if  (written_size  != buffer.size())
        throw std::runtime_error("stream proxy error: written size " +
        convert<string>(written_size) + " != buffer size " + convert<string>(buffer.size())); // ----->

    m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());
}


TBuffer &CStreamProxy::getBuffer() {
    return m_buffer;
}


void CStreamProxy::setReader(IStreamReader::TSharedPtr const &reader) {
    m_reader = reader;
}


void CStreamProxy::setWriter(IStreamWriter::TSharedPtr const &writer) {
    m_writer = writer;
}


} // implementation
} // io
} // utility
