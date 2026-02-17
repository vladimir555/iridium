#ifndef HEADER_MULTIPLEXER_BASE_8C2C1486_C382_4991_ACC1_E2EAF41EC058
#define HEADER_MULTIPLEXER_BASE_8C2C1486_C382_4991_ACC1_E2EAF41EC058


#include "iridium/threading/async_queue.h"
#include "iridium/io/stream.h"
#include "iridium/io/event.h"
#include <unordered_map>


namespace iridium::io::implementation {


class CMultiplexerBase {
public:
    CMultiplexerBase();
    virtual ~CMultiplexerBase() = default;

    std::list<Event::TSharedPtr> finalizeAllEvents();

protected:
    std::unordered_map<uintptr_t, IStream::TSharedPtr>
        m_map_fd_stream;
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr
        m_streams_to_add;
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr
        m_streams_to_del;
    threading::IAsyncQueue<Event::TSharedPtr>::TSharedPtr
        m_wake_events;
    std::atomic<bool>
        m_is_closing;
};


} // iridium::io::implementation


#endif // HEADER_MULTIPLEXER_BASE_8C2C1486_C382_4991_ACC1_E2EAF41EC058
