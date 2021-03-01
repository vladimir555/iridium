// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
#define HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "iridium/io/listener.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <unordered_map>
#include <sys/epoll.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


class CListener:
    public IListener,
    public threading::Synchronized,
    public std::enable_shared_from_this<CListener>
{
public:
    DEFINE_IMPLEMENTATION(CListener)
    CListener();

    void    initialize()   override;
    void    finalize()     override;

    void    add(IStream::TSharedPtr const &stream) override;
    void    del(IStream::TSharedPtr const &stream) override;
    TEvents wait() override;

private:
    static size_t const DEFAULT_EVENTS_COUNT_LIMIT = 2;

    static int assertOK(int const &result, std::string const &message);

    void    addInternal(IStream::TSharedPtr const &stream);
    void    delInternal(IStream::TSharedPtr const &stream);

    std::unordered_map<uintptr_t, IStream::TSharedPtr>
            m_map_fd_stream;
    int     m_epoll_fd;
    int     m_event_fd;
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_streams_to_add;
    threading::IAsyncQueue<IStream::TSharedPtr>::TSharedPtr m_streams_to_del;
};


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
