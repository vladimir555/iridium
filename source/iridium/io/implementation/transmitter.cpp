/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "transmitter.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"


using iridium::convertion::convert;
using iridium::threading::IAsyncQueuePusher;
using std::string;


namespace iridium {
namespace io {
namespace implementation {


size_t const CTransmitter::DEFAULT_BUFFER_SIZE  = 1024 * 1024 * 10;
size_t const CTransmitter::DEFAULT_BUFFER_COUNT = 8;


CTransmitter::CTransmitter(
    IListener::TSharedPtr   const &listener,
    size_t                  const &buffer_size,
    size_t                  const &buffer_count)
:
    m_buffer_size   (buffer_size),
    m_buffer_count  (buffer_count),
    m_listener      (listener)
{}


CTransmitter::~CTransmitter() {
    if (m_reader) {
        m_reader->finalize();
        m_listener->del(m_reader);
    }

    if (m_writer) {
        m_writer->finalize();
        m_listener->del(m_writer);
    }
}


void CTransmitter::setReader(IStreamReader::TSharedPtr const &reader) {
//    LOGT << "fd " << reader->getID();

    // todo: conflict with other transmitters
    if (m_reader) {
        m_listener->del(m_reader);
        //m_reader->finalize();
    }

    m_reader = reader;
    m_listener->add(reader);
}


void CTransmitter::setWriter(IStreamWriter::TSharedPtr const &writer) {
//    LOGT << "fd " << writer->getID();

    // todo: conflict with other transmitters
    if (m_writer) {
        m_listener->del(m_writer);
        //m_writer->finalize();
    }

    m_writer = writer;
    m_listener->add(writer);
}


IStreamReader::TSharedPtr CTransmitter::getReader() const {
    return m_reader; // ----->
}


IStreamWriter::TSharedPtr CTransmitter::getWriter() const {
    return m_writer; // ----->
}


bool CTransmitter::transmit(Event::TSharedPtr const &event) {
    assertExists(m_reader, "transmitter: reader does not exists");
    assertExists(m_writer, "transmitter: writer does not exists");

    LOGT << "fd " << event->stream->getID() << " event " << event->type
         << " "   << m_reader->getID()      << " -> "    << m_writer->getID();

    if (event->type == Event::TType::CLOSE || 
        event->type == Event::TType::ERROR) 
    {
        if (m_reader)
            m_reader->finalize();
        if (m_writer)
            m_writer->finalize();
        return false;
    }

    bool result = true;

    if  (m_buffers.size()  <  m_buffer_count &&
        (m_buffers.empty() || m_reader->getID() == 0 ||
        (m_reader->getID() == event->stream->getID() && 
        (event->type & Event::TType::READ))))
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
        (event->type & Event::TType::WRITE)))))
    {
        auto size =  m_writer->write(*m_buffers.front());
        if  (size == m_buffers.front()->size())
            m_buffers.pop_front();
        else {
            if (size > 0) {
                Buffer::TSharedPtr buffer = m_buffers.front();
                buffer->assign(buffer->begin() + size, buffer->end());
            }
        }

//        LOGT << "wrote " << size;
    }

    return result; // ----->
}
    

} // implementation
} // io
} // iridium
