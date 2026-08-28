#ifndef HEADER_MULTIPLEXER_F5DA57B6_AFA7_4B31_A690_9EB5892AE006
#define HEADER_MULTIPLEXER_F5DA57B6_AFA7_4B31_A690_9EB5892AE006


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/io/implementation/multiplexer_base.h"
#include "iridium/convertion/convert.h"
#include "iridium/threading/synchronized.h"

#include <cstring>
#include <unordered_set>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <unordered_set>


namespace iridium::io::implementation::platform::unix_ {


class CMultiplexer:
    public IMultiplexer,
    public CMultiplexerBase,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)

    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT);

    void initialize() override;
    void finalize() override;

    std::list<Event::TSharedPtr> waitEvents() override;

    void subscribe  (IStream::TSharedPtr const &stream) override;
    void unsubscribe(IStream::TSharedPtr const &stream) override;
    void wake       (Event::TSharedPtr const &event) override;
    void wake       (std::list<Event::TSharedPtr> const &events) override;

private:
    static size_t const DEFAULT_EVENTS_LIMIT = 65535;

    template<typename T>
    static T assertOK(T const &result, std::string const &message);

    int m_wake_pipe[2] =
        { -1, -1 };

    std::vector<struct pollfd>
        m_pollfds;
    std::chrono::microseconds
        m_timeout;

    std::list<Event::TSharedPtr> applyPendingChanges();
    void processWakePipe(std::list<Event::TSharedPtr> &events);
};


template<typename T>
T CMultiplexer::assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " + std::strerror(errno) +
          ", code " + iridium::convertion::convert<std::string>(errno)); // ----->

    return result; // ----->
}


} // iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM
#endif // HEADER_MULTIPLEXER_F5DA57B6_AFA7_4B31_A690_9EB5892AE006
