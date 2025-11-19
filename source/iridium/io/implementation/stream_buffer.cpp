#include "stream_buffer.h"

#include "iridium/assert.h"
#include "iridium/logging/logger.h"


namespace iridium::io::implementation {


CStreamBuffer::CStreamBuffer(Buffer::TSharedPtr const &buffer)
:
    m_buffer(assertExists(buffer, "buffer is null"))
{}


void CStreamBuffer::initialize() {}


void CStreamBuffer::finalize() {}


std::list<uintptr_t> CStreamBuffer::getHandles() const {
    return {}; // ----->
}


URI::TSharedPtr CStreamBuffer::getURI() const {
    return nullptr; // ----->
}


CStreamReaderBuffer::CStreamReaderBuffer(Buffer::TSharedPtr const &buffer)
:
    CStreamBuffer   (buffer),
    m_pos           (0)
{}


Buffer::TSharedPtr CStreamReaderBuffer::read(size_t const &size) {
    if (m_pos < m_buffer->size() && size > 0) {
        auto lpos = m_pos;
        m_pos += size;
        if (m_pos > m_buffer->size())
            m_pos = m_buffer->size();
        return Buffer::create(&(*m_buffer)[lpos], &(*m_buffer)[m_pos]); // ----->
    } else
        return {}; // ----->
}


CStreamWriterBuffer::CStreamWriterBuffer(Buffer::TSharedPtr const &buffer)
:
    CStreamBuffer(buffer)
{}


size_t CStreamWriterBuffer::write(Buffer::TSharedPtr const &buffer) {
    // todo: https://linux.die.net/man/3/open_memstream
    m_buffer->insert(m_buffer->end(), buffer->begin(), buffer->end());
    return buffer->size(); // ----->
}


} // iridium::io::implementation
