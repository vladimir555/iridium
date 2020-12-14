/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "transmitter.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"
#include "iridium/io/service.h"


using iridium::convertion::convert;
using iridium::threading::IAsyncQueuePusher;
using std::string;


namespace iridium {
namespace io {
namespace implementation {


size_t const CTransmitter::DEFAULT_BUFFER_SIZE  = 8192;
size_t const CTransmitter::DEFAULT_BUFFER_COUNT = 8;


CTransmitter::CTransmitter(
    size_t                  const &buffer_size,
    size_t                  const &buffer_count)
:
    m_buffer_size   (buffer_size),
    m_buffer_count  (buffer_count)
{}


CTransmitter::~CTransmitter() {
    if (m_reader) {
        Service::instance().del(m_reader);
        m_reader->finalize();
    }

    if (m_writer) {
        Service::instance().del(m_writer);
        m_writer->finalize();
    }
}


void CTransmitter::set(
    IStreamReader::TSharedPtr const &reader,
    IStreamWriter::TSharedPtr const &writer)
{
    string from, to;

    if (m_reader)
        from += convert<string>(m_reader->getID()) + " ";
    else
        from += "null ";

    if (m_writer)
        from += convert<string>(m_writer->getID()) + " ";
    else
        from += "null ";

    LOGT << "from: " << from;

    if (m_reader &&
        m_reader->getID() > 0 &&
        m_reader->getID() != reader->getID() &&
        m_reader->getID() != writer->getID())
    {
        Service::instance().del(m_reader);
        m_reader->finalize();
    }

    if (m_writer &&
        m_writer->getID() > 0 &&
        m_writer->getID() != reader->getID() &&
        m_writer->getID() != writer->getID())
    {
        Service::instance().del(m_writer);
        m_writer->finalize();
    }

    if (reader && reader->getID() > 0 &&
       (!m_reader ||
       (reader->getID() != m_reader->getID() &&
        reader->getID() != m_writer->getID())))
    {
        reader->initialize();
        Service::instance().add(reader);
    }

    if (writer && writer->getID() > 0 &&
       (!m_writer ||
       (writer->getID() != m_reader->getID() &&
        writer->getID() != m_writer->getID())))
    {
        writer->initialize();
        Service::instance().add(writer);
    }

    m_reader = reader;
    m_writer = writer;

    if (m_reader)
        to += convert<string>(m_reader->getID()) + " ";
    else
        to += "null ";

    if (m_writer)
        to += convert<string>(m_writer->getID()) + " ";
    else
        to += "null ";

    LOGT << "to  : " << to;
}


IStreamReader::TConstSharedPtr CTransmitter::getReader() const {
    return m_reader; // ----->
}


IStreamWriter::TConstSharedPtr CTransmitter::getWriter() const {
    return m_writer; // ----->
}


bool CTransmitter::transmit(Event::TSharedPtr const &event) {
    // todo: pipe for unix
    // todo: openssl pipe https://linux.die.net/man/3/bio_s_bio

    assertExists(m_reader, "transmitter: reader does not exists");
    assertExists(m_writer, "transmitter: writer does not exists");

    LOGT << "fd " << event->stream->getID() << " event " << event->type
         << " "   << m_reader->getID()      << " -> "    << m_writer->getID();

    bool result = true;

    if  (m_buffers.size()  <  m_buffer_count &&
        (m_buffers.empty() || m_reader->getID() == 0 ||
        (m_reader->getID() == event->stream->getID() && 
        (event->type == Event::TType::READ))))
    {
        auto buffer = m_reader->read(m_buffer_size);
        if  (buffer && buffer->size() > 0) {
            m_buffers.push_back(buffer);
//            LOGT << "read " << buffer->size();
        } else {
            result = !m_buffers.empty();
//            LOGT << "read EOF";
        }
    }

    if (!m_buffers.empty() && 
        (m_writer->getID() == 0 || 
       ((m_writer->getID() == event->stream->getID() &&
        (event->type == Event::TType::WRITE)))))
    {
        auto size =  m_writer->write(m_buffers.front());
        if  (size == m_buffers.front()->size()) {
            m_buffers.pop_front();
        } else {
            Buffer::TSharedPtr buffer = m_buffers.front();
            buffer->assign(buffer->begin() + size, buffer->end());
        }

//        LOGT << "wrote " << size;
    }

    return result; // ----->
}
    

} // implementation
} // io
} // iridium
