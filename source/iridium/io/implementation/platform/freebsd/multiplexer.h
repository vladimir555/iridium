// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
#define HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/io/implementation/multiplexer_base.h"
#include "iridium/convertion/convert.h"
#include "iridium/threading/synchronized.h"


#include <sys/event.h>
#include <cstring>


namespace iridium::io::implementation::platform {


class CMultiplexer:
    public IMultiplexer,
    public CMultiplexerBase,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT);

    void initialize()   override;
    void finalize()     override;

    std::list<Event::TSharedPtr> waitEvents() override;

    void subscribe  (IStream::TSharedPtr const &stream) override;
    void unsubscribe(IStream::TSharedPtr const &stream) override;
    void wake       (Event::TSharedPtr const &event) override;
    void wake       (std::list<Event::TSharedPtr> const &events) override;

private:
    static size_t const DEFAULT_EVENTS_LIMIT = 64;

    template<typename T>
    static T assertOK(T const &result, std::string const &message);

    static void handleSignal(int signal);

    void wakeKEvent();

    struct timespec
        m_timeout;

    std::vector<struct kevent>
        m_triggered_events;
    std::atomic<bool>
        m_is_initialized;
    std::atomic<int>
        m_kqueue;
};


template<typename T>
T CMultiplexer::assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " + std::strerror(errno) +
          ", code " + iridium::convertion::convert<std::string>(errno)); // ----->

    return result; // ----->
}


} // iridium::io::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM
#endif // HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
