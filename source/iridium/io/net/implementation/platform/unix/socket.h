// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
#define HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/net/socket.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <cstring>
#include <string>
#include <netinet/in.h>


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix_ {


class CSocketBase: public ISocket, public pattern::NonCopyable {
protected:
    CSocketBase(URI const &uri);
    CSocketBase(int const &socket);
    virtual ~CSocketBase() = default;

    void    open();
    void    close();

    void    listen();
    void    connect();

    int     accept();
    URI     getSocketURI(URI::TProtocol const &protocol);

    size_t  write(Buffer::TSharedPtr const &buffer);
    Buffer::TSharedPtr read(size_t const &size);

    // todo: rename non-blocking -> overlapped
    void    setBlockingMode(int const &socket, bool const &is_blocking);

    URI::TSharedPtr getURI() const override;
    int     getID()  const override;

    int             m_socket;
    URI::TSharedPtr m_uri;
    bool            m_is_opened;

private:
    template<typename T>
    T assertOK(T const &result, std::string const &message);

    static int initSignal();

    static int const    YES;

    struct sockaddr_in  m_address;
};


template<typename T>
T CSocketBase::assertOK(T const &result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_uri)
            throw std::runtime_error(message + ": uri " + convert<string>(*m_uri) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
        else
            throw std::runtime_error(message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    } else
        return result; // ----->
}


} // unix_
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
