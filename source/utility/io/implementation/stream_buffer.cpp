#include "stream_buffer.h"


namespace utility {
namespace io {
namespace implementation {


CStreamReaderBuffer::CStreamReaderBuffer(TBuffer const &buffer)
:
    m_buffer    (buffer),
    m_pos       (0)
{}


TBuffer CStreamReaderBuffer::read(size_t const &size) {
    auto lpos = m_pos < m_buffer.size() ? m_pos : m_buffer.size();
    auto rpos = lpos + size;

    rpos = rpos < m_buffer.size() ? rpos : m_buffer.size();

    return TBuffer(&m_buffer[lpos], &m_buffer[rpos]); // ----->
}


CStreamWriterBuffer::CStreamWriterBuffer(TBuffer &buffer)
:
    m_buffer(buffer)
{}


size_t CStreamWriterBuffer::write(TBuffer const &buffer) {
    m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());
    return buffer.size();
}


} // implementation
} // io
} // utility
