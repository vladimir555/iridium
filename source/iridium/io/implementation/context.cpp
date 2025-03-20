#include "context.h"

#include "iridium/threading/implementation/async_queue.h"
#include "iridium/assert.h"

#include "pipe.h"
#include <algorithm>


using iridium::threading::implementation::CAsyncQueue;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {


CContext::CContext(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol)
:
    m_events    (CAsyncQueue<Event::TSharedPtr>::create()),
    m_protocol  (protocol)
{}


void CContext::pushEvent(Event::TSharedPtr const &event) {
    LOCK_SCOPE();

    auto size = m_events->push(event);
    LOGT << "push event, size: " << size;
}


std::list<Event::TSharedPtr> CContext::popEvents() {
    LOCK_SCOPE();

    auto events = m_events->pop(false);
    auto now    = std::chrono::system_clock::now();
    
    {
//        LOCK_SCOPE(); // timestamp
        for (auto const &event: events)
            m_map_stream_timestamp[event->stream] = now;
    }

    LOGT << "pop  event, size: " << events.size();
    return events; // ----->
}


std::list<Event::TSharedPtr> CContext::checkOutdatedStreams() {
    static std::chrono::seconds const DEFAULT_STREAM_TIMEOUT(10);

    std::list<Event::TSharedPtr> result;
    auto now = std::chrono::system_clock::now();
    
    LOCK_SCOPE(); // timestamp
    for (auto const &stream_timestamp: m_map_stream_timestamp)
        if (now - stream_timestamp.second > DEFAULT_STREAM_TIMEOUT)
            result.push_back(
                Event::create(
                    stream_timestamp.first, 
                    Event::TOperation::TIMEOUT, 
                    Event::TStatus::BEGIN));
    
    return result; // ----->
}


bool CContext::update(Event::TSharedPtr const &event) {
    //LOGT 
    //    << "context update: " 
    //    << event->stream->getHandles().front() << " "
    //    << event->operation << " " 
    //    << event->status;

    if (!m_protocol)
        return true; // ----->
    
    if (event->operation == Event::TOperation::OPEN)
        m_map_stream_pipe[event->stream];
    
    auto result = m_protocol->control(event, shared_from_this());
    
    if (event->operation    == Event::TOperation::CLOSE &&
        event->status       == Event::TStatus::END)
    {
        //LOGT << "auto remove pipe";
        removeStream(event->stream, false);
        if (auto pipe = m_map_stream_pipe[event->stream])
            removePipe(pipe);
    }
        
    return result; // ----->
}


bool CContext::transmit(Event::TSharedPtr const &event) {
    if (!m_protocol)
        return false; // ----->

    return assertExists(
        m_map_stream_pipe[event->stream], 
        "context transmitting error: pipe not found")->transmit(event); // ----->
}


void CContext::createPipe(std::string const &name) {
    if (m_map_name_pipe[name])
        throw std::runtime_error("pipe creating error: '" + name + "' already exists"); // ----->
    
    m_map_name_pipe[name] = CPipe::create();
}


void CContext::removePipe(std::string const &name) {
    LOCK_SCOPE(); // timestamp

    auto pipe = m_map_name_pipe[name];
    if (!pipe)
        throw std::runtime_error("context pipe remove error: pipe '" + name + "' not found"); // ----->

    removePipe(pipe);
    
    m_map_name_pipe.erase(name);
}


void CContext::updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) {
    updatePipe(name, reader, nullptr);
}


void CContext::updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) {
    updatePipe(name, nullptr, writer);
}


void CContext::updatePipe(
    std::string               const &name,
    IStreamReader::TSharedPtr const &reader,
    IStreamWriter::TSharedPtr const &writer)
{
    auto pipe = m_map_name_pipe[name];
    if (!pipe)
        throw std::runtime_error("context pipe update error: pipe '" + name + "' not found"); // ----->
    
    if (reader && reader->getURI() && m_map_stream_pipe.find(reader) == m_map_stream_pipe.end()) {
        m_events->push(Event::create(reader, Event::TOperation::OPEN, Event::TStatus::BEGIN));
        //LOGT
        //    << "update pipe, event: "
        //    << Event::TOperation::OPEN << " "
        //    << Event::TStatus::BEGIN << " "
        //    << reader->getHandles().front();
    } else
    if (reader && reader->getURI()) {
        //LOGT
        //    << "update pipe, event: "
        //    << Event::TOperation::READ << " "
        //    << Event::TStatus::BEGIN << " "
        //    << reader->getHandles().front();
        m_events->push(Event::create(reader, Event::TOperation::READ, Event::TStatus::BEGIN));
    }

    if (writer && writer->getURI() && m_map_stream_pipe.find(writer) == m_map_stream_pipe.end()) {
        m_events->push(Event::create(writer, Event::TOperation::OPEN,  Event::TStatus::BEGIN));
        //LOGT
        //    << "update pipe, event: "
        //    << Event::TOperation::OPEN << " "
        //    << Event::TStatus::BEGIN << " "
        //    << writer->getHandles().front();
    } else
    if (writer && writer->getURI()) {
        //LOGT 
        //    << "update pipe, event: " 
        //    << Event::TOperation::WRITE << " " 
        //    << Event::TStatus::BEGIN << " "
        //    << writer->getHandles().front();

        m_events->push(Event::create(writer, Event::TOperation::WRITE, Event::TStatus::BEGIN));
    }

    m_map_stream_pipe[reader] = pipe;
    m_map_stream_pipe[writer] = pipe;

    pipe->set(reader, writer);
}


void CContext::removePipe(IPipe::TSharedPtr const &pipe) {
    if (!pipe)
        throw std::runtime_error("context pipe remove error: pipe not found"); // ----->

    removeStream(pipe->getReader());
    removeStream(pipe->getWriter());
}


void CContext::remove() {
//    LOGT << "CContext::remove";
    for (auto const &stream_pipe: m_map_stream_pipe) {
        if (!stream_pipe.first->getHandles().empty()) {
            LOGT
                << "CContext::remove push event: "
                << stream_pipe.first->getHandles()
                << " " << Event::TOperation::CLOSE
                << " " << Event::TStatus::BEGIN;
            m_events->push(Event::create(stream_pipe.first, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
        }
    }
//    for (auto const &stream_pipe: m_map_stream_pipe)
//        stream_pipe.first->finalize();

    m_map_stream_pipe.clear();
    m_map_name_pipe.clear();
    m_protocol.reset();
    LOCK_SCOPE();
    m_map_stream_timestamp.clear();
}


void CContext::removeStream(IStream::TSharedPtr const &stream, bool const &is_send_close_event) {
    if (stream && stream->getURI()) {
        auto i  = m_map_stream_pipe.find(stream);
        if  (i != m_map_stream_pipe.end()) {
            m_map_stream_pipe.erase(i);
            if (is_send_close_event) {
//                LOGT << "remove stream: event close, id: " << stream->getID();
                LOGT << "push Event::TOperation::CLOSE, fd: " << stream->getHandles();
                m_events->push(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
            }
        }
    }
    LOCK_SCOPE();
    m_map_stream_timestamp.erase(stream);
}


} // implementation
} // io
} // iridium
