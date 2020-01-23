/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream_buffer.h"

#include "iridium/assert.h"
#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {


CStreamReaderBuffer::CStreamReaderBuffer(Buffer::TSharedPtr const &buffer)
:
    m_buffer    (assertExists(buffer, "buffer is null")),
    m_pos       (0)
{}

    
void CStreamReaderBuffer::initialize() {}


void CStreamReaderBuffer::finalize() {}


int CStreamReaderBuffer::getID() const {
    return {}; // ----->
}
    

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
    m_buffer(assertExists(buffer, "buffer is null"))
{}

    
void CStreamWriterBuffer::initialize() {}


void CStreamWriterBuffer::finalize() {}


int  CStreamWriterBuffer::getID() const {
    return {}; //----->
}


size_t CStreamWriterBuffer::write(Buffer const &buffer) {
    m_buffer->insert(m_buffer->end(), buffer.begin(), buffer.end());
    return buffer.size(); // ----->
}


} // implementation
} // io
} // iridium
