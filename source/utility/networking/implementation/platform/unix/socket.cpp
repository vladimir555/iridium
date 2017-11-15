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
#include <signal.h>

#include "utility/encryption/implementation/ssl.h"
#include "utility/threading/synchronized_scope.h"
#include "utility/assert.h"
#include "utility/logging/logger.h"

#include <iostream>


using std::string;
using utility::convertion::convert;
using utility::convertion::convertPtr;
using utility::encryption::implementation::CContext;


namespace {


static int const DEFAULT_SOCKET_BUFFER_SIZE = 512;


// todo: signals handlers singleton
void handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
}


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
    m_encryptor         (nullptr),
    m_acceptor          (nullptr)
{}


CSocket::~CSocket() {
}


void CSocket::open() {
   auto protocol = IPPROTO_TCP;
    if (!m_url.getProtocol() || *m_url.getProtocol() == URL::TProtocol::UDP)
        protocol = IPPROTO_UDP;

    m_socket_fd = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket open error");
}


void CSocket::close() {
//    LOGT << m_url << " " << static_cast<int>(m_socket_fd);
    if (m_ssl)
        m_ssl.reset();
    if (m_acceptor)
        m_acceptor->remove(this);
    assertOK(::close(m_socket_fd), "close socket error");
}


void CSocket::write(TPacket const &packet) {
    LOCK_SCOPE;
//    LOGT << m_url << " " << static_cast<int>(m_socket_fd);
    if (m_ssl) {
        m_ssl->write(packet);
    } else {
        size_t lpos = 0;
        while (lpos < packet.size()) {
            auto buffer = static_cast<void const *>(packet.data() + lpos);
            auto rpos   = lpos + DEFAULT_SOCKET_BUFFER_SIZE;
            if (rpos > packet.size())
                rpos = packet.size();
            lpos += assertOK(::send(m_socket_fd, buffer, rpos - lpos, 0), "socket write error");
//            LOGT << "lpos " << lpos;
        }
    }
}


CSocket::TPacket CSocket::read() {
//    LOGT << m_url << " " << m_socket_fd;
    LOCK_SCOPE
    TPacket result;
    if (m_ssl) {
        result = m_ssl->read(DEFAULT_SOCKET_BUFFER_SIZE); // ----->
    } else {
        char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
        auto received_size = assertOK(::recv(m_socket_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0), "socket read error");
//        LOGT << m_url << " " << m_socket_fd << " received_size " << received_size;
        result = TPacket(buffer, buffer + received_size); // ----->
    }
    return result; // ----->
}


void CSocket::listen() {
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

    assertOK(::bind  (m_socket_fd, (struct sockaddr *) (&server_address), sizeof(server_address)), "socket bind error");
    assertOK(::listen(m_socket_fd, SOMAXCONN), "socket listen error");

    if (!m_encryptor && m_url.getProtocol() && *m_url.getProtocol() == URL::TProtocol::HTTPS)
        m_encryptor = CContext::create(m_is_blocking_mode);

    LOGT << "set empty signal handler for broken pipe";
    struct sigaction sh;
    struct sigaction osh;

    // can set to SIG_IGN
    sh.sa_handler   = &handleSignal;
    // restart interrupted system calls
    sh.sa_flags     = SA_RESTART;

    // block every signal during the handler
    sigemptyset(&sh.sa_mask);

    if (sigaction(SIGPIPE, &sh, &osh) < 0)
        throw std::runtime_error("sigaction error");
}


ISocket::TSocketStreams CSocket::accept() {
    // todo: thread for every accepted socket
    ISocket::TSocketStreams sockets;
//    for (auto const &socket: acceptInternal())
//        sockets.push_back(ISocketStream::TSharedPtr(new unix::CSocket(socket, getPeerURL(socket), this)));

    return sockets; // ----->
}


void CSocket::interrupt() {
    ::shutdown(m_socket_fd, 2);
}


void CSocket::connect() {
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
    LOCK_SCOPE;
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
    auto flags = assertOK(fcntl(m_socket_fd, F_GETFL, 0), "socket get flag error");
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_socket_fd, F_SETFL, flags), "socket set flag error");
    m_is_blocking_mode = is_blocking;
}


std::list<int> CSocket::acceptInternal() {
    std::list<int>      sockets;
    struct sockaddr_in  client_address      = { 0 };
    socklen_t           client_address_size = sizeof(client_address);
    int                 socket              = 0;

    do {
        socket = ::accept(m_socket_fd, (struct sockaddr *) (&client_address), &client_address_size);
        if (socket > 0)
            sockets.push_back(socket);
    } while (socket > 0);

    return sockets; // ----->
}


CSocket::TSharedPtr CSocket::createInternal(int const &socket_fd) {
    LOCK_SCOPE;
    auto const url = getPeerURL(socket_fd);
    for (auto const &i: m_accepted_sockets) {
        if (i->getURL() == url) {
            LOGT << "update fd " << i->m_socket_fd << " -> " << socket_fd;

            assertOK(::close(i->m_socket_fd), "close socket error");

            i->m_socket_fd  = socket_fd;

            if (m_encryptor)
                i->m_ssl    = m_encryptor->accept(m_socket_fd);

            return nullptr; // ----->
        }
    }

    auto socket         = create(url);

    socket->m_acceptor  = this;
    socket->m_socket_fd = socket_fd;
    socket->setBlockingMode(false);

    m_accepted_sockets.push_back(socket);

//    LOGT << "url " << socket->getURL() << " " << socket->m_socket_fd;

    return socket; // ----->
}


void CSocket::remove(CSocket const * const accepted_socket) {
    auto url = assertExists(accepted_socket, "accepted_socket is null")->getURL();
    LOCK_SCOPE;
    for (auto const &i: m_accepted_sockets) {
        if (i->getURL() == url) {
//            LOGT << url << " " << i->m_socket_fd;
            m_accepted_sockets.remove(i);
            return; // ----->
        }
    }
//    LOGT << "sockets size " << m_accepted_sockets.size();
}


} // unix
} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // UNIX_PLATFORM
