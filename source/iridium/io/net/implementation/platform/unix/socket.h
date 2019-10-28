#ifndef HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
#define HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/net/url.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


class CSocket: public ISocket {
public:
    DEFINE_IMPLEMENTATION(CSocket)
    CSocket(URL const &url, bool const &is_server_mode);

    void    initialize() override;
    void    finalize() override;

    IStream::TSharedPtr accept() override;
    URL     getURL() const override;

    int     getID() const override;

    size_t  write(Buffer const &buffer) override;
    Buffer  read(size_t const &size) override;
    
//    void    flush() override;

private:
    CSocket(URL const &url, int const &fd);

    URL     getPeerURL(int const &fd);
    void    setBlockingMode(bool const &is_blocking);

    bool    m_is_server_mode;
    URL     m_url;
    int     m_socket;
};


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
