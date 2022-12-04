#ifndef HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
#define HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/convertion/convert.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/worker.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>

#include <sys/event.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


// todo: rename CMultiplexer
class CMultiplexer: public IMultiplexer, public threading::Synchronized {
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT);

    void initialize()   override;
    void finalize()     override;

    std::list<IEvent::TSharedPtr> waitEvents() override;

    void subscribe  (IStream::TSharedPtr const &stream) override;
    void unsubscribe(IStream::TSharedPtr const &stream) override;

private:
    static size_t const DEFAULT_EVENTS_LIMIT = 4;

    template<typename T>
    static T assertOK(T const &result, std::string const &message);

    static void handleSignal(int signal);

    std::array<int, 2> registerPipe();

    struct timespec m_timeout;

    std::vector<struct kevent> m_triggered_events;

    int m_kqueue;
    std::array<int, 2> m_pipe_add;
    std::array<int, 2> m_pipe_del;

    std::unordered_map<uintptr_t, IStream::TSharedPtr> m_map_fd_stream;
};


template<typename T>
T CMultiplexer::assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " + std::strerror(errno) +
          ", code " + iridium::convertion::convert<std::string>(errno)); // ----->
    else
        return result; // ----->
}


} // platform
} // implementation
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
#endif // HEADER_EVENT_PROVIDER_14F82211_CB2B_4426_94FE_0D72FE64652E
