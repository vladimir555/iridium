#include "transmitter.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"


using iridium::convertion::convert;
using std::string;


namespace iridium {
namespace io {
namespace implementation {


CTransmitter::CTransmitter(
    IStreamReader::TSharedPtr   const &reader,
    IStreamWriter::TSharedPtr   const &writer,
    size_t                      const &buffer_size
):
    m_reader        (reader),
    m_writer        (writer),
    m_buffer_size   (buffer_size)
{}


//void CTransmitter::setReader(IStream::TSharedPtr const &reader) {
//    m_reader = reader;
//}
//
//
//void CTransmitter::setWriter(IStream::TSharedPtr const &writer) {
//    m_writer = writer;
//}


//IStream::TSharedPtr CTransmitter::getReader() const {
//    return m_reader; // ----->
//}
//
//
//IStream::TSharedPtr CTransmitter::getWriter() const {
//    return m_writer; // ----->
//}


bool CTransmitter::transmit() {
    
    auto buffer = m_reader->read(m_buffer_size);

    LOGT << "read  '" << buffer << "' buffer.size = " << buffer.size();

    if (buffer.empty())
        return false; // ----->

    auto size = m_writer->write(buffer);

    LOGT << "write '" << buffer << "'";

    if (size < buffer.size())
        throw std::runtime_error(
            "transmitter error: write size "    + convert<string>(size) +
            " < read size "                     + convert<string>(buffer.size())); // ----->
    else
        return true; // ----->

    
//    auto buffer = m_reader->read(m_buffer_size);
//
//    LOGT << "read  '" << buffer << "' buffer.size = " << buffer.size();
//
//    if (buffer.empty())
//        return false; // ----->
//
//    auto size = m_writer->write(buffer);
//
//    LOGT << "write '" << buffer << "'";
//
//    if (size < buffer.size())
//        throw std::runtime_error(
//            "stream proxy error: write size "   + convert<string>(size) +
//            " < read size "                     + convert<string>(buffer.size())); // ----->
//    else
//        return true; // ----->
}
    

} // implementation
} // io
} // iridium
