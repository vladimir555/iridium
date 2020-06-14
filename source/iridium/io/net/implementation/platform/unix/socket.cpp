/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "socket.h"


#ifdef UNIX_PLATFORM


#include <sys/socket.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <vector>

#include "iridium/logging/logger.h"

#include <signal.h>

#include <linux/tls.h>
#include <netinet/tcp.h>
using iridium::io::Buffer;
using iridium::io::IStream;
using iridium::convertion::convert;
using iridium::io::net::URL;
using std::string;
using std::vector;


static size_t const DEFAULT_SOCKET_BUFFER_SIZE = 8192; // todo: one buffer


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocket::CSocket(URL const &url, bool const &is_server_mode)
:
    m_is_server_mode(is_server_mode),
    m_url           (url),
    m_socket        (0),
    m_address       ({})
{}


CSocket::CSocket(URL const &url, int const &fd)
:
    m_is_server_mode(false),
    m_url           (url),
    m_socket        (fd),
    m_address       ({})
{}


URL CSocket::getPeerURL(int const &fd) {
    struct sockaddr_in   peer;
    unsigned int         peer_len = sizeof(peer);

    if (::getpeername(fd, (sockaddr *)&peer, &peer_len) < 0)
        throw std::runtime_error("socket get peer ip error for socket fd " +
        convert<string>(m_socket)); // ----->

   auto protocol =  m_url.getProtocol();
    if (protocol == URL::TProtocol::UNKNOWN)
        protocol =  URL::TProtocol::TCP;

    return URL(convert<string>(protocol) + "://" + inet_ntoa(peer.sin_addr) + ":" +
        convert<string>(ntohs(peer.sin_port))); // ----->
}


void CSocket::initialize() {
    // open
    auto protocol   = m_url.getProtocol() == URL::TProtocol::UDP ? IPPROTO_UDP : IPPROTO_TCP;
    m_socket        = assertOK(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, protocol), "socket open error", m_url);
    LOGT << "fd " << m_socket;

    //todo: unix / inet protocol by url
//    struct sockaddr_in address  = {};
    auto &address = m_address = {};
    auto ipv4 = *m_url.getIPv4();
    address.sin_addr.s_addr     = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    address.sin_family          = AF_INET;
    address.sin_port            = htons( *m_url.getPort() );

    if (m_is_server_mode) {
        int yes = 1;
        // todo: TCP_NODELAY
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

        assertOK(::bind   (m_socket, (struct sockaddr *) &address, sizeof(address)), "socket bind error", m_url);
        assertOK(::listen (m_socket, SOMAXCONN), "socket listen error", m_url);
    } else {
//        assertOK(::connect(m_socket, (struct sockaddr *) &address, sizeof(address)), "socket connect error", m_url);
    }
    setBlockingMode(false);

    // todo: make static
    signal(SIGPIPE, SIG_IGN);
}


void CSocket::finalize() {
    LOGT << "fd " << m_socket;
    ::shutdown(m_socket, 2);
    assertOK(::close(m_socket), "close socket error", m_url);
}


ISocket::TSharedPtr CSocket::accept() {
    struct sockaddr_in  peer_address      = {};
    socklen_t           peer_address_size = sizeof(peer_address);

    auto peer_fd = ::accept(m_socket, (struct sockaddr *) &peer_address, &peer_address_size);
    if  (peer_fd > 0) {
        LOGT << "fd " << peer_fd;
        auto peer = new unix::CSocket(getPeerURL(peer_fd), peer_fd);
        peer->setBlockingMode(false);
        return ISocket::TSharedPtr(peer); // ----->
    } else
        return nullptr; // ----->
}


URL CSocket::getURL() const {
    return m_url;       // ----->
}


int CSocket::getID() const {
    return m_socket;    // ----->
}


size_t CSocket::write(Buffer::TSharedPtr const &buffer_) {
    LOGT << "fd " << m_socket << " size " << buffer_->size();
    auto buffer = static_cast<void const *>(buffer_->data());
    auto size   = DEFAULT_SOCKET_BUFFER_SIZE;
    
    if (size > buffer_->size())
        size = buffer_->size();
    
    auto result = ::send(m_socket, buffer, size, 0);
    LOGT << "fd " << m_socket << " size " << result << " sent";

    if (result < 0) {
        if (errno == EAGAIN)
            return 0;
        else
            assertOK(result, "socket write error", m_url);
    }

    // was sended async
    if (result == EAGAIN ||
        result == EWOULDBLOCK
    )
        return
        buffer_->size() < DEFAULT_SOCKET_BUFFER_SIZE ?
        buffer_->size() : DEFAULT_SOCKET_BUFFER_SIZE; // ----->
    else
        return result; // ----->

    return buffer_->size(); // ----->
}


Buffer::TSharedPtr CSocket::read(size_t const &size_) {
    auto const size = size_ > 0 ? size_: DEFAULT_SOCKET_BUFFER_SIZE;
    char buffer[size];
    auto received_size  = ::recv(m_socket, buffer, size - 1, 0);
    LOGT << "fd " << m_socket << " size " << received_size;

//    if (m_is_blocking) {
//        assertOK(received_size, "socket read error", m_url);
//    } else {
//        if (received_size == EOF)
//            return {}; // ----->
//        else
//            assertOK(received_size, "socket read error", m_url);
//    }

    if (received_size == EOF) {
//        LOGT << "EOF";
        return {};
    }

    if(!(received_size == EOF && !m_is_blocking))
        assertOK(received_size, "socket read error", m_url);
    else
        received_size = 0;

//    LOGT << "fd " << m_socket << " " << Buffer(buffer, buffer + received_size);
//    LOGT << "received_size  = " << received_size;
    return Buffer::create(Buffer(buffer, buffer + received_size)); // ----->
}
    
    
//void CSocket::flush() {
//// todo:
////    flush(m_socket);
//}


void CSocket::setBlockingMode(bool const &is_blocking) {
    auto flags = assertOK(fcntl(m_socket, F_GETFL, 0), "socket get flag error", m_url);
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_socket, F_SETFL, flags), "socket set flag error", m_url);
    m_is_blocking = is_blocking;
}


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
