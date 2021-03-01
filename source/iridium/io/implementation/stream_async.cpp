#include "stream_async.h"


namespace iridium {
namespace io {
namespace implementation {


CAsyncStream::CAsyncStream(IStream::TSharedPtr const &stream, IListener::TSharedPtr const &listener)
:
    m_stream    (stream),
    m_listener  (listener)
{}


int CAsyncStream::getID() const {
    return m_stream->getID(); // ----->
}


void CAsyncStream::initialize() {
    m_stream->initialize();
    m_listener->add(m_stream);
}


void CAsyncStream::finalize() {
    m_listener->del(m_stream);
    m_stream->finalize();
}


CAsyncStreamReader::CAsyncStreamReader(
    IStreamReader::TSharedPtr   const &stream,
    IListener::TSharedPtr       const &listener)
:
    CAsyncStream(stream, listener),
    m_stream    (stream)
{}


Buffer::TSharedPtr CAsyncStreamReader::read(size_t const &size) {
    return m_stream->read(size); // ----->
}


CAsyncStreamWriter::CAsyncStreamWriter(
    IStreamWriter::TSharedPtr   const &stream,
    IListener::TSharedPtr       const &listener)
:
    CAsyncStream(stream, listener),
    m_stream    (stream)
{}


size_t CAsyncStreamWriter::write(Buffer::TSharedPtr const &buffer) {
    return m_stream->write(buffer); // ----->
}


} // implementation
} // io
} // iridium
