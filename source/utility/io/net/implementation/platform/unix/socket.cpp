#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>

#include "utility/convertion/convert.h"


using utility::io::TBuffer;
using utility::io::IStream;
using utility::convertion::convert;
using utility::io::net::URL;
using std::string;


template<typename T>
T assertOK(T const &result, string const &message, URL const &url) {
    if (result < 0)
        throw std::runtime_error(message + ": url " + convert<string>(url) +
            ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    else
        return result; // ----->
}


static int const DEFAULT_SOCKET_BUFFER_SIZE = 8;//8912;


namespace utility {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocket::CSocket(URL const &url, bool const &is_server_mode)
:
    m_url               (url),
    m_is_server_mode    (is_server_mode),
    m_socket            (0)
{}


CSocket::CSocket(int const &fd)
:
    m_url   (""),
    m_socket(fd)
{
    struct sockaddr_in   peer;
    unsigned int         peer_len = sizeof(peer);

    if (::getpeername(m_socket, (sockaddr *)&peer, &peer_len) < 0)
        throw std::runtime_error("socket get peer ip error for socket fd " +
        convert<string>(socket)); // ----->

    auto protocol = convert<string>(URL::TProtocol::TCP);
    if (m_url.getProtocol())
        protocol  = convert<string>(m_url.getProtocol());
    m_url = URL(protocol + "://" + inet_ntoa(peer.sin_addr) + ":" +
        convert<string>(ntohs(peer.sin_port))); // ----->
}


void CSocket::initialize() {
    // open
    auto protocol   = IPPROTO_TCP;
    if (m_url.getProtocol() == URL::TProtocol::UDP)
        protocol    = IPPROTO_UDP;

    m_socket        = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket open error", m_url);

    //todo: unix / inet protocol by url
    struct sockaddr_in address  = { 0 };
    auto ipv4 = *m_url.getIPv4();
    address.sin_addr.s_addr     = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    address.sin_family          = AF_INET;
    address.sin_port            = htons( *m_url.getPort() );

    if (m_is_server_mode) {
        // bind, listen

        int yes = 1;
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        assertOK(::bind  (m_socket, (struct sockaddr *) (&address), sizeof(address)), "socket bind error", m_url);
        assertOK(::listen(m_socket, SOMAXCONN), "socket listen error", m_url);
    } else {
        // connect

        assertOK(::connect(m_socket, (struct sockaddr *) &address, sizeof(address)), "socket connect error", m_url);
    }
    setBlockingMode(false);
}


void CSocket::finalize() {
    ::shutdown(m_socket, 2);
    assertOK(::close(m_socket), "close socket error", m_url);
}


IStream::TSharedPtr CSocket::accept() {
    struct sockaddr_in  peer_address      = { 0 };
    socklen_t           peer_address_size = sizeof(peer_address);

    auto peer_fd = ::accept(m_socket, (struct sockaddr *) (&peer_address), &peer_address_size);
    if  (peer_fd > 0) {
        auto peer = new unix::CSocket(peer_fd);
        peer->setBlockingMode(false);
        return IStream::TSharedPtr(peer); // ----->
    } else
        return nullptr; // ----->
}


URL CSocket::getURL() const {
    return m_url; // ----->
}


int CSocket::getID() const {
    return m_socket; // ----->
}


size_t CSocket::write(TBuffer const &packet) {
    auto buffer = static_cast<void const *>(packet.data());
    auto result = ::send(m_socket, buffer, DEFAULT_SOCKET_BUFFER_SIZE, 0);

    // was sended async
    if (result == EAGAIN || result == EWOULDBLOCK)
        return packet.size() < DEFAULT_SOCKET_BUFFER_SIZE ? packet.size() : DEFAULT_SOCKET_BUFFER_SIZE; // ----->
    else
        return result; // ----->

    return packet.size(); // ----->
}


TBuffer CSocket::read(size_t const &size) {
    char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
    auto received_size = assertOK(::recv(m_socket, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0), "socket read error", m_url);
    return TBuffer(buffer, buffer + received_size); // ----->
}


void CSocket::setBlockingMode(bool const &is_blocking) {
    auto flags = assertOK(fcntl(m_socket, F_GETFL, 0), "socket get flag error", m_url);
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_socket, F_SETFL, flags), "socket set flag error", m_url);
}


} // unix
} // platform
} // implementation
} // net
} // io
} // utility
