// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "context_.h"

#include "iridium/threading/implementation/async_queue.h"
#include "iridium/assert.h"

#include "pipe.h"
#include <algorithm>


using iridium::threading::implementation::CAsyncQueue;


#include "iridium/logging/logger.h"
namespace iridium::io::implementation {


CContext_::CContext_(IStream::TSharedPtr const &, IProtocol::TSharedPtr const &protocol)
:
    m_events    (CAsyncQueue<Event::TSharedPtr>::create()),
    m_protocol  (protocol)
{}


void CContext_::pushEvent(Event::TSharedPtr const &event) {
    LOCK_SCOPE();

    m_events->push(event);
    // LOGT << "push event: " << event;
}


std::list<Event::TSharedPtr> CContext_::popEvents() {
    LOCK_SCOPE();

    auto events = m_events->pop(false);
    auto now    = std::chrono::system_clock::now();

    {
//        LOCK_SCOPE(); // timestamp
        for (auto const &event: events)
            m_map_stream_timestamp[event->stream] = now;
    }

    // LOGT << "pop events: " << events;
    return events; // ----->
}


std::list<Event::TSharedPtr> CContext_::checkOutdatedStreams() {
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


bool CContext_::update(Event::TSharedPtr const &event) {
    //LOGT
    //    << "context update: "
    //    << event->stream->getHandles().front() << " "
    //    << event->operation << " "
    //    << event->status;

    if (!m_protocol)
        return true; // ----->

    if (event->operation == Event::TOperation::OPEN)
        m_map_stream_pipe[event->stream];

    // LOGT << "[PROTOCOL] control called with: " << event->operation << " " << event->status;
    auto result = m_protocol->control(event, shared_from_this());
    // LOGT << "[PROTOCOL] control returned: " << result;

    if (event->operation    == Event::TOperation::CLOSE &&
        event->status       == Event::TStatus::END)
    {
        // LOGT << "[CLEANUP] removing pipe for stream";
        removeStream(event->stream, false);
        if (auto pipe = m_map_stream_pipe[event->stream])
            removePipe(pipe);
    }

    return result; // ----->
}


// bool CContext::transmit(Event::TSharedPtr const &event) {
//     // LOGT << "[TRANSMIT_PIPE] op: " << event->operation << " has_protocol: " << (m_protocol != nullptr);
//     if (!m_protocol)
//         return false; // ----->

//     auto pipe = m_map_stream_pipe[event->stream];
//     if  (pipe) {
//         return pipe->transmit(event); // ----->
//     } else {
//         // if (event->operation == Event::TOperation::CLOSE)
//             return false; // ----->

//         // throw std::runtime_error("context transmitting error: pipe not found"); // ----->
//     }
// }


IPipe::TSharedPtr CContext_::getPipe(Event::TSharedPtr const &event) {
    if (m_protocol) {
        auto   stream_pipe =  m_map_stream_pipe.find(event->stream);
        return stream_pipe == m_map_stream_pipe.end() ? nullptr : stream_pipe->second; // ----->
    }

    return {};
}


void CContext_::createPipe(std::string const &name) {
    //LOGT << "create pipe: " << name;
    if (m_map_name_pipe[name])
        throw std::runtime_error("pipe creating error: '" + name + "' already exists"); // ----->

    m_map_name_pipe[name] = CPipe::create();
}


void CContext_::removePipe(std::string const &name) {
    LOCK_SCOPE(); // timestamp

    auto pipe = m_map_name_pipe[name];
    if (!pipe)
        throw std::runtime_error("context pipe remove error: pipe '" + name + "' not found"); // ----->

    removePipe(pipe);

    m_map_name_pipe.erase(name);
}


void CContext_::updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) {
    updatePipe(name, reader, nullptr);
}


void CContext_::updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) {
    updatePipe(name, nullptr, writer);
}


void CContext_::updatePipe(
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

    if (reader)
        m_map_stream_pipe[reader] = pipe;
    if (writer)
        m_map_stream_pipe[writer] = pipe;

    pipe->set(reader, writer);
}


void CContext_::removePipe(IPipe::TSharedPtr const &pipe) {
    //LOGT
        //<<"remove pipe: "
        //<< (pipe->getReader() ? pipe->getReader()->getHandles() : std::list<uintptr_t>{}) << " "
        //<< (pipe->getWriter() ? pipe->getWriter()->getHandles() : std::list<uintptr_t>{});

    if (!pipe)
        throw std::runtime_error("context pipe remove error: pipe not found"); // ----->

    removeStream(pipe->getReader());
    removeStream(pipe->getWriter());
}


void CContext_::remove() {
//    LOGT << "CContext::remove";
//    for (auto const &stream_pipe: m_map_stream_pipe) {
//        if (!stream_pipe.first->getHandles().empty()) {
//            LOGT
//                << "CContext::remove push event: "
//                << stream_pipe.first->getHandles()
//                << " " << Event::TOperation::CLOSE
//                << " " << Event::TStatus::BEGIN;
//            m_events->push(Event::create(stream_pipe.first, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
//        }
//    }
//    for (auto const &stream_pipe: m_map_stream_pipe)
//        stream_pipe.first->finalize();

    // Don't clear pipes here - they may still be needed for events in queue
    // m_map_stream_pipe.clear();
    // m_map_name_pipe.clear();
    m_protocol.reset();
    LOCK_SCOPE();
    m_map_stream_timestamp.clear();
}


void CContext_::removeStream(IStream::TSharedPtr const &stream, bool const &is_send_close_event) {
    if (stream && !stream->getHandles().empty() &&stream->getURI()) {
        auto i  = m_map_stream_pipe.find(stream);
        if  (i != m_map_stream_pipe.end()) {
            m_map_stream_pipe.erase(i);
            if (is_send_close_event) {
//                LOGT << "remove stream: event close, id: " << stream->getID();
                //LOGT << "push Event::TOperation::CLOSE, fd: " << stream->getHandles();
                m_events->push(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
            }
        }
    }
    LOCK_SCOPE();
    m_map_stream_timestamp.erase(stream);
}


} // iridium::io::implementation
