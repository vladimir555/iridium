// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
#define HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/net/url.h"
#include "iridium/io/net/socket.h"
#include "iridium/convertion/convert.h"

#include <cstring>
#include <string>
#include <netinet/in.h>


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

    ISocket::TSharedPtr accept() override;
    URL     getURL() const override;

    int     getID() const override;

    size_t  write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr read(size_t const &size) override;
    
//    void    flush() override;

protected:
    CSocket(URL const &url, int const &fd);

    template<typename T>
    T assertOK(T const &result, std::string const &message, URL const &url) {
        using convertion::convert;
        using std::string;
        if (result < 0)
            throw std::runtime_error(message + ": url " + convert<string>(url) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
        else
            return result; // ----->
    }

    URL     getPeerURL(int const &fd);
    void    setBlockingMode(bool const &is_blocking);

    bool    m_is_blocking;
    bool    m_is_server_mode;
    URL     m_url;
    int     m_socket;
    struct sockaddr_in m_address;
};


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
