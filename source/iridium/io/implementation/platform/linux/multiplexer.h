// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
#define HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "iridium/threading/synchronized.h"
#include "iridium/io/multiplexer.h"
#include "iridium/io/implementation/multiplexer_base.h"

#include <sys/epoll.h>


namespace iridium::io::implementation::platform {


// todo: fix handling orphan sockets on lost connection
class CMultiplexer:
    public IMultiplexer,
    public CMultiplexerBase,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    CMultiplexer();

    void initialize()   override;
    void finalize()     override;

    void    subscribe(IStream::TSharedPtr const &stream) override;
    void  unsubscribe(IStream::TSharedPtr const &stream) override;

    std::list<Event::TSharedPtr> waitEvents() override;

    void wake(Event::TSharedPtr const &event) override;
    void wake(std::list<Event::TSharedPtr> const &events) override;

private:
    static size_t const DEFAULT_EVENTS_COUNT_LIMIT = 2;

    static int assertOK(int const &result, std::string const &message);

    void addInternal(IStream::TSharedPtr const &stream);
    void delInternal(IStream::TSharedPtr const &stream);

    std::atomic<int>
        m_epoll_fd;
    std::atomic<bool>
        m_is_closing;
    int m_event_fd;
};


} // iridium::io::implementation::platform


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
