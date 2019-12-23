/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream_reader_list.h"

#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {


CStreamReaderList::CStreamReaderList()
:
    m_is_started(false)
{}


void CStreamReaderList::initialize() {}
    
    
void CStreamReaderList::finalize() {}


Buffer CStreamReaderList::read(size_t const &size) {
    if(!m_is_started) {
        m_is_started = true;
        m_stream     = m_streams.begin();
    }
    while (m_stream != m_streams.end()) {
        auto result  = m_stream->get()->read(size);
        if  (result.empty()) {
//            LOGT << "next stream";
            m_stream++;
            continue; // <---
        } else {
//            LOGT << "return: " << result;
            return result; // ----->
        }
    }
//    LOGT << "return: empty";
    return {}; // ----->
}
    
    
int CStreamReaderList::getID() const {
    if (m_is_started)
        return m_stream->get()->getID(); // ----->
    else
        return {}; // ----->
}


void CStreamReaderList::add(IStreamReader::TSharedPtr const &stream_reader) {
    m_streams.push_back(stream_reader);
//    LOGT << "add stream, size = " << m_streams.size();
}


} // implementation
} // io
} // iridium
