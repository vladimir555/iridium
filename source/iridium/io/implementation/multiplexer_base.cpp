#include "multiplexer_base.h"


#include "iridium/threading/implementation/async_queue.h"


using iridium::threading::implementation::CAsyncQueue;


namespace iridium::io::implementation {


CMultiplexerBase::CMultiplexerBase()
:
    m_streams_to_add
        (CAsyncQueue<IStream::TSharedPtr>::create()),
    m_streams_to_del
        (CAsyncQueue<IStream::TSharedPtr>::create()),
    m_wake_events
        (CAsyncQueue<Event::TSharedPtr>::create())
{}


std::list<Event::TSharedPtr> CMultiplexerBase::finalizeAllEvents() {
    std::list<Event::TSharedPtr> events;

    for (auto const &stream: m_streams_to_del->pop(false))
        events.push_back(
            Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));

    for (auto const &stream: m_streams_to_add->pop(false))
        events.push_back(
            Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));

    for (auto const &fd_stream: m_map_fd_stream) {
        if (fd_stream.second)
            events.push_back(
                Event::create(fd_stream.second, Event::TOperation::CLOSE, Event::TStatus::END));
    }

    events.splice(events.end(), m_wake_events->pop(false));

    m_map_fd_stream.clear();

    return events; // ----->
}


} // iridium::io::implementation
