// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "multiplexer_base.h"


#include "iridium/threading/implementation/async_queue.h"


using iridium::threading::implementation::CAsyncQueue;


namespace iridium::io::implementation {


CMultiplexerBase::CMultiplexerBase()
:
    m_streams_to_handle
        (CAsyncQueue<TStreamToHandle>::create()),
    m_wake_events
        (CAsyncQueue<Event::TSharedPtr>::create()),
    m_is_initialized
        (false)
{}


std::list<Event::TSharedPtr> CMultiplexerBase::finalizeAllEvents() {
    std::list<Event::TSharedPtr> events;

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
