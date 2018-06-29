#ifndef HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
#define HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F


#include "utility/platform.h"


#ifdef UNIX_PLATFORM


#include "utility/io/net/url.h"
#include "utility/io/net/socket.h"


namespace utility {
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

    io::IStream::TSharedPtr accept() override;
    URL     getURL() const override;

    int     getID() const override;

    size_t  write(io::TBuffer const &packet) override;
    io::TBuffer read(size_t const &size) override;

private:
    CSocket(int const &fd);

    void    setBlockingMode(bool const &is_blocking);

    URL     m_url;
    bool    m_is_server_mode;
    int     m_socket;
};


} // unix
} // platform
} // implementation
} // net
} // io
} // utility


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
