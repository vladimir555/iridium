#include "utility/platform.h"



#ifdef UNIX_PLATFORM


#include "socket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "utility/encryption/implementation/ssl.h"
#include "utility/threading/synchronized_scope.h"
#include "utility/logging/logger.h"


using std::string;
using utility::convertion::convert;
using utility::convertion::convertPtr;
using utility::encryption::implementation::CContext;


namespace {


static int const DEFAULT_SOCKET_BUFFER_SIZE = 512;


} // unnamed


namespace utility {
namespace networking {
namespace implementation {
namespace platform {
namespace unix {


CSocket::CSocket(URL const &url)
:
    m_is_blocking_mode  (false),
    m_socket_fd         (0),
    m_url               (url),
    m_encryptor         (nullptr)
{
//    LOCK_SCOPE;
}


CSocket::CSocket(int const &socket, URL const &url, unix::CSocket *acceptor)
:
    m_is_blocking_mode  (false),
    m_socket_fd         (socket),
    m_url               (url),
    m_encryptor         (acceptor->m_encryptor),
    m_acceptor          (acceptor)
{
//    LOCK_SCOPE;
    if (m_encryptor)
        m_ssl = m_encryptor->accept(m_socket_fd);
}


CSocket::~CSocket() {
//    interrupt();
}


void CSocket::open() {
    LOCK_SCOPE;
   auto protocol = IPPROTO_TCP;
    if (!m_url.getProtocol() || *m_url.getProtocol() == URL::TProtocol::UDP)
        protocol = IPPROTO_UDP;

    m_socket_fd = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket open error");
}


void CSocket::close() {
    LOCK_SCOPE;
    if (m_ssl)
        m_ssl.reset();
    assertOK(::close(m_socket_fd), "close socket error");
    if (m_acceptor)
        m_acceptor->removeURLFromMap(m_url);
}


void CSocket::write(TPacket const &packet) {
    LOGT << m_url << " " << m_socket_fd;
    LOCK_SCOPE;
    if (m_ssl) {
        m_ssl->write(packet);
    } else {
        size_t lpos = 0;
        while (lpos < packet.size()) {
            auto buffer = static_cast<void const *>(packet.data() + lpos);
            auto rpos = lpos + DEFAULT_SOCKET_BUFFER_SIZE;
            if (rpos > packet.size())
                rpos = packet.size();
            lpos += assertOK(::send(m_socket_fd, buffer, rpos - lpos, 0), "socket write error");
            LOGT << "lpos " << lpos;
        }
    }
}


CSocket::TPacket CSocket::read() {
    LOGT << m_url << " " << m_socket_fd;
    LOCK_SCOPE;
    TPacket result;
    if (m_ssl) {
        result = m_ssl->read(DEFAULT_SOCKET_BUFFER_SIZE); // ----->
    } else {
        char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
        auto received_size = assertOK(::recv(m_socket_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0), "socket read error");
        LOGT << "received_size " << received_size;
        result = TPacket(buffer, buffer + received_size); // ----->
    }
    return result; // ----->
}


void CSocket::listen() {
//    LOCK_SCOPE;
    //todo: unix / inet protocol by url
    struct sockaddr_in server_address = { 0 };

    server_address.sin_family       = AF_INET;
//    server_address.sin_addr.s_addr  = INADDR_ANY;
    auto ipv4 = *m_url.getIPv4();
    server_address.sin_addr.s_addr  = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    server_address.sin_port         = htons(*m_url.getPort());

    int yes = 1;
    setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    assertOK(::bind(m_socket_fd, (struct sockaddr *) (&server_address), sizeof(server_address)), "socket bind error");
    assertOK(::listen(m_socket_fd, SOMAXCONN), "socket listen error");

    if (!m_encryptor && m_url.getProtocol() && *m_url.getProtocol() == URL::TProtocol::HTTPS)
        m_encryptor = CContext::create(m_is_blocking_mode);
}


ISocket::TSocketStreams CSocket::accept() {
    // todo: thread for every accepted socket
    ISocket::TSocketStreams sockets;
    for (auto const &socket: acceptInternal())
        sockets.push_back(ISocketStream::TSharedPtr(new unix::CSocket(socket, getPeerURL(socket), this)));

    return sockets; // ----->
}


void CSocket::interrupt() {
    LOCK_SCOPE;
    ::shutdown(m_socket_fd, 2);
}


void CSocket::connect() {
    LOCK_SCOPE;
    struct sockaddr_in address;
    auto ipv4 = *m_url.getIPv4();
    address.sin_addr.s_addr  = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    address.sin_family       = AF_INET;
    address.sin_port         = htons( *m_url.getPort() );

    assertOK(::connect(m_socket_fd, (struct sockaddr *) &address, sizeof(address)), "socket connect error");
}


URL CSocket::getURL() const {
    return m_url;
}


int CSocket::assertOK(int const &result, std::string const &message) const {
    if (result < 0)
        throw std::runtime_error(message + ": url " + convert<string>(m_url) + ", " + std::strerror(errno)); // ----->
    else
        return result; // ----->
}


URL CSocket::getPeerURL(int const &socket) {
    struct sockaddr_in   peer;
    unsigned int         peer_len = sizeof(peer);
    if (::getpeername(socket, (sockaddr *)&peer, &peer_len) < 0)
        throw std::runtime_error("socket get peer ip error for socket fd " + convert<string>(socket)); // ----->

    //LOGT << "m_url: " << m_url;
    auto protocol   = convert<string>(URL::TProtocol::TCP);
    if (m_url.getProtocol())
        protocol    = convert<string>(*m_url.getProtocol());
    return URL(protocol + "://" + inet_ntoa(peer.sin_addr) + ":" + convert<string>(ntohs(peer.sin_port))); // ----->
}


void CSocket::setBlockingMode(bool const &is_blocking) {
    LOCK_SCOPE;
    auto flags = assertOK(fcntl(m_socket_fd, F_GETFL, 0), "socket get flag error");
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_socket_fd, F_SETFL, flags), "socket set flag error");
    m_is_blocking_mode = is_blocking;
}


std::list<int> CSocket::acceptInternal() {
    LOCK_SCOPE;
    std::list<int>      sockets;
    struct sockaddr_in  client_address      = { 0 };
    socklen_t           client_address_size = sizeof(client_address);
    int                 socket              = 0;

    do {
        socket = ::accept(m_socket_fd, (struct sockaddr *) (&client_address), &client_address_size);
        if (socket > 0) {
            auto i = m_map_url_socket.find(getPeerURL(socket));
            if (i == m_map_url_socket.end())
                sockets.push_back(socket);
            else
                i->second->updateAcceptedSocketFD(socket);
        }
    } while (socket > 0);

    return sockets; // ----->
}


void CSocket::updateAcceptedSocketFD(int const &socket_fd) {
    LOCK_SCOPE;
    LOGT << m_url << " update fd " << m_socket_fd << " -> " << socket_fd;
    m_socket_fd = socket_fd;
    if (m_ssl && m_encryptor)
        m_ssl = m_encryptor->accept(m_socket_fd);
}


void CSocket::removeURLFromMap(URL const &url) {
    LOCK_SCOPE;
    m_map_url_socket.erase(url);
}


} // unix
} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // UNIX_PLATFORM
