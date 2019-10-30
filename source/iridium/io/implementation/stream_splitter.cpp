/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream_splitter.h"


namespace iridium {
namespace io {
namespace implementation {


CStreamSplitter::CStreamSplitter(
    IStreamReader::TSharedPtr const &reader,
    IStreamWriter::TSharedPtr const &writer)
{}

    
int CStreamSplitter::getID() const {
    return 0;
}
    
    
void CStreamSplitter::initialize() {
    m_reader->initialize();
    m_writer->initialize();
}

    
void CStreamSplitter::finalize() {
    m_reader->finalize();
    m_writer->finalize();
}

    
void CStreamSplitter::set(IStreamReader::TSharedPtr const &reader) {
    m_reader = reader;
}


void CStreamSplitter::set(IStreamWriter::TSharedPtr const &writer) {
    m_writer = writer;
}

    
Buffer CStreamSplitter::read(size_t const &size) {
    if (m_reader)
        return m_reader->read(size); // ----->
    else
        throw std::runtime_error("stream splitter error: reader is not initialized");
}

    
size_t CStreamSplitter::write(Buffer const &buffer) {
    if (m_writer)
        return m_writer->write(buffer); // ----->
    else
        throw std::runtime_error("stream splitter error: writer is not initialized");
}


} // implementation
} // io
} // iridium
