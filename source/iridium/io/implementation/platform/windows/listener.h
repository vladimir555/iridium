// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LISTENER_324A28CA_8E92_4C84_BE9F_6BF4899756D4
#define HEADER_LISTENER_324A28CA_8E92_4C84_BE9F_6BF4899756D4


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/listener.h"

//#include <unordered_map>


namespace iridium {
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

//private:
//    std::unordered_map<uintptr_t, IStreamPort::TSharedPtr>
//            m_map_fd_stream;
//    int     m_epoll_fd;
};


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_LISTENER_324A28CA_8E92_4C84_BE9F_6BF4899756D4
