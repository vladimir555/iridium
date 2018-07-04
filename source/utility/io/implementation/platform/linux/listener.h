#ifndef HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
#define HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80


#include "utility/platform.h"


#ifdef LINUX_PLATFORM


#include "utility/io/listener.h"
#include <unordered_map>


namespace utility {
namespace io {
namespace implementation {
namespace platform {


class CListener: public IListener {
public:
    DEFINE_IMPLEMENTATION(CListener)
    CListener();

    void    initialize()   override;
    void    finalize()     override;

    void    add(IStream::TSharedPtr const &stream) override;
    void    del(IStream::TSharedPtr const &stream) override;
    TEvents wait() override;

private:
    std::unordered_map<uintptr_t, IStream::TSharedPtr> m_map_fd_stream;
};


} // platform
} // implementation
} // io
} // utility


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_4F88765B_9F8A_4F8A_8622_061022685F80
