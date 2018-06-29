#include "utility/platform.h"



#ifdef UNIX_PLATFORM


#ifndef HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12
#define HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#include "utility/net/socket.h"
#include "utility/net/url.h"
#include "utility/encryption/ssl.h"
#include "utility/threading/implementation/recursive_mutex.h"
#include "utility/threading/implementation/timed_mutex.h"

#include <map>
#include <memory>
#include <atomic>


namespace utility {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


class CSocket: public ISocket {
public:
    DEFINE_IMPLEMENTATION(CSocket)
    CSocket(URL const &url);

    void            open() override;
    void            close() override;
    size_t          write(TPacket const &packet) override;
    TPacket         read() override;
    void            listen() override;
    TEvents         accept() override;
    void            interrupt() override;
    void            connect() override;
    URL             getURL() const override;

protected:
    template<typename T>
    T               assertOK(T const &result, std::string const &message) const;
    URL             getPeerURL(int const &socket);
    void            setBlockingMode(bool const &is_blocking);

    TEvents         acceptInternal(std::list<int> &accepted_sockets_fd);
//    CSocket::TSharedPtr     findAcceptedSocket(int const &socket_fd);

    bool            m_is_blocking_mode;
    int             m_socket_fd;
    URL const       m_url;

    encryption::IContext::TSharedPtr    m_encryptor;
    encryption::ISSL::TSharedPtr        m_ssl;

private:
//    void remove(CSocket const * const accepted_socket);

//    std::list<CSocket::TSharedPtr>  m_accepted_sockets;
//    CSocket                        *m_acceptor;
};


template<typename T>
T CSocket::assertOK(T const &result, std::string const &message) const {
    if (result < 0)
        throw std::runtime_error(message + ": url " + convertion::convert<std::string>(m_url) +
            ", " + std::strerror(errno) + ", code " + convertion::convert<std::string>(errno)); // ----->
    else
        return result; // ----->
}


} // unix
} // platform
} // implementation
} // net
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // UNIX_PLATFORM
