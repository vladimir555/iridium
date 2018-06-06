#ifndef HEADER_LISTENER_C34DC1E3_F80D_41A8_8CCC_08D026E35B74
#define HEADER_LISTENER_C34DC1E3_F80D_41A8_8CCC_08D026E35B74


#include "utility/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "utility/io/listener.h"

#include <sys/event.h>

#include <unordered_map>


namespace utility {
namespace io {
namespace implementation {
namespace platform {


class CListener: public IListener {
public:
    DEFINE_IMPLEMENTATION(CListener)
    CListener();

    void initialize()   override;
    void finalize()     override;

    void add(IStream::TSharedPtr const &stream) override;
    void del(IStream::TSharedPtr const &stream) override;
    TEvents wait()                              override;

private:
    std::vector<struct kevent>  m_events;
    std::vector<struct kevent>  m_monitor_events;
    int                         m_monitor_events_used_count;
    int                         m_kqueue;
    std::unordered_map<int, IStream::TSharedPtr> m_map_fd_stream;
};


} // platform
} // implementation
} // io
} // utility


#endif // FREEBSD_LIKE_PLATFORM
#endif // HEADER_LISTENER_C34DC1E3_F80D_41A8_8CCC_08D026E35B74