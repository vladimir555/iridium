#include "context.h"

#include "iridium/threading/implementation/async_queue.h"
#include "iridium/assert.h"

#include "pipe.h"


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
    m_events->push(event);
}


std::list<Event::TSharedPtr> CContext::popEvents() {
    auto events = m_events->pop(false); // ----->
    auto now    = std::chrono::system_clock::now();
    
    {
        LOCK_SCOPE(); // timestamp
        for (auto const &event: events)
            m_map_stream_timestamp[event->stream] = now;
    }
    
    // remove duplicates
    auto i = std::unique(events.begin(), events.end(),
        [] (auto const &l, auto const &r) {
            return
                l && r &&
                l->stream->getID()  == r->stream->getID() &&
                l->operation        == r->operation &&
                l->status           == r->status;
        }
    );
    events.erase(i, events.end());

    return events;
}


std::list<Event::TSharedPtr> CContext::checkOutdatedStreams() {
    static std::chrono::seconds const DEFAULT_STREAM_TIMEOUT(10);

    std::list<Event::TSharedPtr> result;
    auto now = std::chrono::system_clock::now();
    
    LOCK_SCOPE(); // timestamp
    for (auto const &stream_timestamp: m_map_stream_timestamp)
        if (now - stream_timestamp.second > DEFAULT_STREAM_TIMEOUT)
            result.push_back(Event::create(stream_timestamp.first, Event::TOperation::TIMEOUT));
    
    return result; // ----->
}


bool CContext::update(Event::TSharedPtr const &event) {
    if (!m_protocol)
        return true; // ----->
    LOGT << "context::update, event: " << event->operation << " " << event->status << " " << event->stream->getID();
    
    if (event->operation == Event::TOperation::OPEN)
        m_map_stream_pipe[event->stream];
    
    auto result = m_protocol->control(event, shared_from_this());
    
    if (event->operation    == Event::TOperation::CLOSE &&
        event->status       == Event::TStatus::END)
    {
        LOGT << "auto remove pipe";
        removeStream(event->stream, false);
        if (auto pipe = m_map_stream_pipe[event->stream])
            removePipe(pipe);
    }
        
    return result; // ----->
}


bool CContext::transmit(Event::TSharedPtr const &event) {
    if (!m_protocol)
        return false; // ----->
    LOGT << "context::transmit, event: " << event->operation << " " << event->status << " " << event->stream->getID();
    return assertExists(m_map_stream_pipe[event->stream], "context transmitting error: pipe not found")->transmit(event);
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
    throw std::runtime_error("not implemented");
//    auto pipe = m_map_name_pipe[name];
//    if (!pipe)
//        throw std::runtime_error("context pipe update error: pipe '" + name + "' not found"); // ----->
//
//    if (m_map_stream_pipe.find(reader) == m_map_stream_pipe.end() && reader && reader->getURI()) {
//        m_events->push(Event::create(reader, Event::TOperation::OPEN, Event::TStatus::BEGIN));
//        LOGT << "update pipe: event open, id: " << reader->getID();
//    }
//
//    if (reader && reader->getURI()) {
//        LOGT << "update pipe: event read, id: " << reader->getID();
//        m_events->push(Event::create(reader, Event::TOperation::READ, Event::TStatus::BEGIN));
//    }
//
//    m_map_stream_pipe[reader] = pipe;
//
//    pipe->set(reader, pipe->getWriter());
}


void CContext::updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) {
    throw std::runtime_error("not implemented");
//    auto pipe = m_map_name_pipe[name];
//    if (!pipe)
//        throw std::runtime_error("context pipe update error: pipe '" + name + "' not found"); // ----->
//
//    if (m_map_stream_pipe.find(writer) == m_map_stream_pipe.end() && writer && writer->getURI()) {
//        m_events->push(Event::create(writer, Event::TOperation::OPEN,  Event::TStatus::BEGIN));
//        LOGT << "update pipe: event open, id: " << writer->getID();
//    }
//
//    if (writer && writer->getURI()) {
//        LOGT << "update pipe: event write, id: " << writer->getID();
//        m_events->push(Event::create(writer, Event::TOperation::WRITE, Event::TStatus::BEGIN));
//    }
//
//    m_map_stream_pipe[writer] = pipe;
//
//    pipe->set(pipe->getReader(), writer);
}


void CContext::updatePipe(
    std::string               const &name,
    IStreamReader::TSharedPtr const &reader,
    IStreamWriter::TSharedPtr const &writer)
{
    auto pipe = m_map_name_pipe[name];
    if (!pipe)
        throw std::runtime_error("context pipe update error: pipe '" + name + "' not found"); // ----->
    
    if (m_map_stream_pipe.find(reader) == m_map_stream_pipe.end() && reader && reader->getURI()) {
        m_events->push(Event::create(reader, Event::TOperation::OPEN, Event::TStatus::BEGIN));
        LOGT << "update pipe: event open, id: " << reader->getID();
    }
    
    else
        
    if (reader && reader->getURI()) {
        LOGT << "update pipe: event read, id: " << reader->getID();
        m_events->push(Event::create(reader, Event::TOperation::READ, Event::TStatus::BEGIN));
    }

    if (m_map_stream_pipe.find(writer) == m_map_stream_pipe.end() && writer && writer->getURI()) {
        m_events->push(Event::create(writer, Event::TOperation::OPEN,  Event::TStatus::BEGIN));
        LOGT << "update pipe: event open, id: " << writer->getID();
    }
    
    else
    
    if (writer && writer->getURI()) {
        LOGT << "update pipe: event write, id: " << writer->getID();
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
    for (auto const &stream_pipe: m_map_stream_pipe)
        m_events->push(Event::create(stream_pipe.first, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
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
