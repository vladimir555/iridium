#include "stream_buffer.h"

#include "iridium/assert.h"
#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {


CStreamReaderBuffer::CStreamReaderBuffer(
//    IStreamReader::TSharedPtr   const &reader,
    Buffer::TSharedPtr          const &buffer)
:
//    m_reader    (assertExists(reader , "stream reader is null")),
    m_buffer    (assertExists(buffer        , "buffer is null")),
    m_pos       (0)
{}

    
void CStreamReaderBuffer::initialize() {}


void CStreamReaderBuffer::finalize() {}


int CStreamReaderBuffer::getID() const {
    return {}; // ----->
}
    

Buffer CStreamReaderBuffer::read(size_t const &size) {
//    auto buffer = m_reader->read(size);
//    m_buffer->insert(m_buffer->end(), buffer.begin(), buffer.end());
//    return buffer; // ----->

    if (m_pos < m_buffer->size() && size > 0) {
        auto lpos = m_pos;
        m_pos += size;
        if (m_pos > m_buffer->size())
            m_pos = m_buffer->size();
        return Buffer(&(*m_buffer)[lpos], &(*m_buffer)[m_pos]); // ----->
    } else
        return Buffer();
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
    
    
//void CStreamWriterBuffer::flush() {
//}


} // implementation
} // io
} // iridium
