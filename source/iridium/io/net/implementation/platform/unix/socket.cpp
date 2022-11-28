/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "socket.h"


#ifdef UNIX_PLATFORM


#include "iridium/assert.h"
#include "iridium/logging/logger.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <signal.h>
#include <netinet/tcp.h>



using iridium::io::Buffer;
using iridium::io::IStream;
using iridium::convertion::convert;
using iridium::io::URL;
using std::string;
using std::vector;


static size_t const DEFAULT_SOCKET_BUFFER_SIZE = 8192; // todo: one buffer


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix_ {


CSocketBase::CSocketBase(URL const &url)
:
    m_socket        (0),
    m_url           (URL::create(url)),
    m_is_opened     (false),
    m_address       ({})
{}


CSocketBase::CSocketBase(int const &socket)
:
    m_socket        (socket),
    m_is_opened     (true),
    m_address       ({})
{}


int CSocketBase::initSignal() {
    signal(SIGPIPE, SIG_IGN);
    return 1; // ----->
}


int const CSocketBase::YES = CSocketBase::initSignal();


void CSocketBase::open() {
    assertExists(m_url, "socket opening error: url is not set");

    if (m_is_opened)
        throw std::runtime_error("socket open error: already opened " + convert<string>(*m_url)); // ----->

    auto ipv4       = *assertExists(m_url->getIPv4(), "socket opening error, wrong url " + convert<string>(*m_url));
    auto port       = *assertExists(m_url->getPort(), "socket opening error, wrong url " + convert<string>(*m_url));
    auto protocol   = m_url->getProtocol() == URL::TProtocol::UDP ? IPPROTO_UDP : IPPROTO_TCP;
    auto &address   = m_address = {};

    m_socket        = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket opening error " + convert<string>(*m_url));

    LOGT << "fd " << m_socket;

    address.sin_addr.s_addr     = htonl(
        ( ipv4[0] << 24 ) |
        ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |
          ipv4[3]);
    address.sin_family          = AF_INET;
    address.sin_port            = htons(port);
    m_is_opened                 = true;
}


void CSocketBase::close() {
    if (!m_is_opened)
        throw std::runtime_error("socket close error: already closed " + convert<string>(*m_url)); // ----->

    LOGT << "fd " << m_socket;
    ::shutdown(m_socket, 2);
    assertOK(::close(m_socket), "socket closing error, fd " + convert<string>(m_socket));
    m_socket    = 0;
    m_is_opened = false;
}


void CSocketBase::listen() {
    // todo: TCP_NODELAY
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES));

    assertOK(::bind   (m_socket, (struct sockaddr *) &m_address, sizeof(m_address)), "socket bind error");
    assertOK(::listen (m_socket, SOMAXCONN), "socket listen error");
    setBlockingMode(m_socket, false);
}


void CSocketBase::connect() {
    // instead SOL_TCP
    setsockopt(m_socket, SOL_SOCKET, TCP_NODELAY, &YES, sizeof(YES));
    assertOK(::connect(m_socket, (struct sockaddr *) &m_address, sizeof(m_address)), "socket connect error");
    setBlockingMode(m_socket, false);
}


int CSocketBase::accept() {
    struct sockaddr_in  peer_address      = {};
    socklen_t           peer_address_size = sizeof(peer_address);

    auto peer_fd = ::accept(m_socket, (struct sockaddr *) &peer_address, &peer_address_size);
    if  (peer_fd > 0) {
        LOGT << "fd " << m_socket << " accepted " << peer_fd;
        setBlockingMode(peer_fd, false);
        return peer_fd; // ----->
    } else
        return 0; // ----->
}


URL CSocketBase::getSocketURL(URL::TProtocol const &protocol) {
    struct sockaddr_in   peer;
    unsigned int         peer_len = sizeof(peer);

    if (::getpeername(m_socket, (sockaddr *)&peer, &peer_len) < 0)
        throw std::runtime_error("socket geting peer ip error for socket fd " +
            convert<string>(m_socket)); // ----->

    return URL(
        convert<string>(protocol) + "://" + inet_ntoa(peer.sin_addr) + ":" +
        convert<string>(ntohs(peer.sin_port))); // ----->
}


size_t CSocketBase::write(Buffer::TSharedPtr const &buffer_) {
    auto buffer = static_cast<void const *>(buffer_->data());
    auto size   = DEFAULT_SOCKET_BUFFER_SIZE;

    if (size > buffer_->size())
        size = buffer_->size();

    auto result = ::send(m_socket, buffer, size, 0);

    if (result < 0) {
        if (errno == EAGAIN)
            return 0;
        else
            assertOK(result, "socket write error");
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


Buffer::TSharedPtr CSocketBase::read(size_t const &size_) {
    auto const size = size_ > 0 ? size_: DEFAULT_SOCKET_BUFFER_SIZE;
    char buffer[size];
    auto received_size  = ::recv(m_socket, buffer, size - 1, 0);

    if(received_size == EOF)
        assertOK(received_size, "socket read error");
    else
        received_size = 0;

    return Buffer::create(Buffer(buffer, buffer + received_size)); // ----->
}


void CSocketBase::setBlockingMode(int const &socket, bool const &is_blocking) {
    LOGT << socket << " " << is_blocking;
    auto flags = assertOK(fcntl(socket, F_GETFL, 0), "socket get flag error");
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(socket, F_SETFL, flags), "socket set flag error");
}


URL CSocketBase::getURL() const {
    if (m_url)
        return *m_url; // ----->
    else
        return URL(""); // ----->
}


int CSocketBase::getID() const {
    return m_socket;
}


} // unix_
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
