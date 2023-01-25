// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
#define HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <unordered_map>
#include <sys/epoll.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


class CMultiplexer:
    public IMultiplexer,
    public threading::Synchronized,
    public std::enable_shared_from_this<CMultiplexer>
{
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    CMultiplexer();

    void    initialize()   override;
    void    finalize()     override;

    void    subscribe  (IStream::TConstSharedPtr const &stream) override;
    void    unsubscribe(IStream::TConstSharedPtr const &stream) override;
    std::list<IEvent::TSharedPtr> waitEvents() override;

private:
    static size_t const DEFAULT_EVENTS_COUNT_LIMIT = 2;

    static int assertOK(int const &result, std::string const &message);

    void    addInternal(IStream::TConstSharedPtr const &stream);
    void    delInternal(IStream::TConstSharedPtr const &stream);

    std::unordered_map<uintptr_t, IStream::TConstSharedPtr>
            m_map_fd_stream;
    int     m_epoll_fd;
    int     m_event_fd;
    threading::IAsyncQueue<IStream::TConstSharedPtr>::TSharedPtr m_streams_to_add;
    threading::IAsyncQueue<IStream::TConstSharedPtr>::TSharedPtr m_streams_to_del;
};


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
