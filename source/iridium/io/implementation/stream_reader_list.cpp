/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream_reader_list.h"

#include "iridium/assert.h"


namespace iridium {
namespace io {
namespace implementation {


CStreamReaderList::CStreamReaderList()
{}


void CStreamReaderList::initialize() {}
    
    
void CStreamReaderList::finalize() {}


Buffer::TSharedPtr CStreamReaderList::read(size_t const &size) {
    if (m_streams.empty() || m_stream == m_streams.end())
        return {}; // ----->

    while (m_stream != m_streams.end()) {
        auto result  = m_stream->get()->read(size);
        if  (result) {
            return result; // ----->
        } else {
            m_stream++;
            continue; // <---
        }
    }
    return {}; // ----->
}
    
    
int CStreamReaderList::getID() const {
    if (m_streams.empty() || m_stream == m_streams.end())
        return {}; // ----->
    else
        return m_stream->get()->getID(); // ----->
}


void CStreamReaderList::add(IStreamReader::TSharedPtr const &stream_reader) {
    bool is_empty = m_streams.empty();
    m_streams.push_back(stream_reader);
    if  (is_empty)
        m_stream  = m_streams.begin();
}


} // implementation
} // io
} // iridium
