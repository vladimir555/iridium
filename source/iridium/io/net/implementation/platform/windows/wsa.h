// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#ifndef HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246
#define HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246


#include "iridium/pattern/singleton.h"
#include "iridium/io/uri.h"
#include "iridium/io/buffer.h"
#include "iridium/smart_ptr.h"

#include <string>
#include <list>

#include <ws2tcpip.h>


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


class WSA: public iridium::pattern::Singleton<WSA> {
public:
    virtual ~WSA();

    URI::TIPv4   getIPv4ByName(std::string const &name);

    Buffer  read(SOCKET const &socket, size_t const &size);
    size_t  write(SOCKET const &socket, Buffer const &packet);
    void    close(SOCKET const &socket);
    SOCKET  connect(URI const &uri);
    SOCKET  listen(URI const &uri);
    SOCKET  accept(SOCKET const &socket);
    void    shutdown(SOCKET const &socket);
    void    setBlockingMode(SOCKET const &socket, bool const &is_blocking);

private:
    friend class iridium::pattern::Singleton<WSA>;

    WSA();

    std::string getLastWSAErrorString() const;

    template<typename T>
    void assertEQ(T const &result, T const &value, std::string const &message) const;
    template<typename T>
    T &assertNE(T &&result, T const &value, std::string const &message) const;

    WSADATA m_wsa_data;
};


// implementation


template<typename T>
void WSA::assertEQ(T const &result, T const &value, std::string const &message) const {
    if (result != value)
        throw std::runtime_error(message + ": " + getLastWSAErrorString()); // ----->
}


template<typename T>
T &WSA::assertNE(T &&result, T const &value, std::string const &message) const {
    if (result == value)
        throw std::runtime_error(message + ": " + getLastWSAErrorString()); // ----->
    else
        return result; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246


#endif // WINDOWS_PLATFORM
