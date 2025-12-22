#include "pipe.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"
#include "iridium/items.h"


using iridium::convertion::convert;
using std::string;


namespace iridium::io::implementation {


size_t const CPipe::DEFAULT_BUFFER_SIZE  = 248;
size_t const CPipe::DEFAULT_BUFFER_COUNT = 8;


CPipe::CPipe()
:
    m_buffer_size   (DEFAULT_BUFFER_SIZE),
    m_buffer_count  (DEFAULT_BUFFER_COUNT)
{}


CPipe::~CPipe() {
////    LOGT;
//    if (m_reader) {
////        LOGT << "del " << m_reader->getID();
//        m_reader->finalize();
//    }

//    if (m_writer) {
////        LOGT << "del " << m_writer->getID();
//        m_writer->finalize();
//    }
}


void CPipe::set(
    IStreamReader::TSharedPtr const &reader,
    IStreamWriter::TSharedPtr const &writer)
{
    string from, to;

    //if (m_reader)
    //    from += convert<string>(uint64_t(m_reader->getID())) + " ";
    //else
    //    from += "null ";

    //if (m_writer)
    //    from += convert<string>(uint64_t(m_writer->getID())) + " ";
    //else
    //    from += "null ";

//    LOGT << "from: " << from;

//    if (m_reader && !checkOneOf<IStream::TSharedPtr>(m_reader, reader, writer)) {
//        LOGT << "finalize m_reader fd " << m_reader->getID();
//        m_reader->finalize();
//    }

//    if (m_writer && !checkOneOf<IStream::TSharedPtr>(m_writer, reader, writer)) {
//        LOGT << "finalize m_writer fd " << m_writer->getID();
//        m_writer->finalize();
//    }

//    if (reader && !checkOneOf<IStream::TSharedPtr>(reader, m_reader, m_writer)) {
//        reader->initialize();
//        LOGT << "initialize reader fd " << reader->getID();
//    }

//    if (writer && !checkOneOf<IStream::TSharedPtr>(writer, m_reader, m_writer)) {
//        writer->initialize();
//        LOGT << "initialize writer fd " << writer->getID();
//    }

    m_reader = reader;
    m_writer = writer;

    //if (m_reader)
    //    to += convert<string>(uint64_t(m_reader->getID())) + " ";
    //else
    //    to += "null ";

    //if (m_writer)
    //    to += convert<string>(uint64_t(m_writer->getID())) + " ";
    //else
    //    to += "null ";

//    LOGT << "to  : " << to;
}


IStreamReader::TSharedPtr CPipe::getReader() const {
    return m_reader; // ----->
}


IStreamWriter::TSharedPtr CPipe::getWriter() const {
    return m_writer; // ----->
}


bool CPipe::transmit(Event::TConstSharedPtr const &event) {
    // todo: pipe for unix
    // todo: openssl pipe https://linux.die.net/man/3/bio_s_bio

    assertExists(m_reader, "pipe: reader does not exists");
    assertExists(m_writer, "pipe: writer does not exists");

    //LOGT << "pipe transmit, " << event->status << " " << event->operation << " " << event->stream->getID()
    //     << ": " << m_reader->getID() << " -> " << m_writer->getID();

    bool result = false;

//    if  (m_buffers.size()    < m_buffer_count &&
//       (!m_reader->getURI() ||
//        (m_reader->getID()  == event->stream->getID() &&
//        (checkOneOf(event->operation, Event::TOperation::READ, Event::TOperation::CLOSE, Event::TOperation::TIMEOUT)))))
    if ( m_buffers.size() < m_buffer_count &&
        (m_reader->getHandles().empty() ||
        (m_reader == event->stream &&
        checkOneOf(
            event->operation,
            Event::TOperation::READ,
            Event::TOperation::CLOSE,
//            Event::TOperation::EOF_,
            Event::TOperation::TIMEOUT)))
    ) {
        //LOGT << "do read";
        auto buffer = m_reader->read(m_buffer_size);

        //LOGT << "read buffer size: " << buffer->size();
        if  (buffer && buffer->size() > 0) {
            m_buffers.push_back(buffer);
            result |= true;
            //LOGT << "read " << buffer->size();
        } else {
            //LOGT << "read EOF";
        }
    }

//    if  (!m_buffers.empty() &&
//        (!m_writer->getURI() ||
//        ((m_writer->getID()  == event->stream->getID() &&
//         (checkOneOf(event->operation, Event::TOperation::WRITE, Event::TOperation::TIMEOUT))))))
    if (!m_buffers.empty() &&
        (m_writer->getHandles().empty() ||
        (m_writer == event->stream &&
        checkOneOf(
            event->operation,
            Event::TOperation::WRITE,
            Event::TOperation::TIMEOUT))))
    {
//        LOGT << "do write";
        auto size =  m_writer->write(m_buffers.front());
        if (size < 0)
            return false; // ----->
        result |= size > 0;
        if  (size == m_buffers.front()->size()) {
            m_buffers.pop_front();
        } else {
            // todo: optimize
            auto buffer = m_buffers.front();
            m_buffers.pop_front();
            if (buffer->size() > size) {
                buffer = Buffer::create(buffer->begin() + size, buffer->end());
                m_buffers.push_front(buffer);
            }
        }

//        LOGT << "wrote " << size;

//        LOGT << "buffers size: " << m_buffers.size() << ", size: " << size;
//        if (m_buffers.empty() && size <= 0)
//            result = false;
    }

//    LOGT << "result = " << result;
    return result; // ----->
}


} // iridium::io::implementation
