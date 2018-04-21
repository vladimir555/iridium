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


static int const DEFAULT_SOCKET_BUFFER_SIZE = 8;//8912;


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
    m_is_blocking_mode  (true),
    m_socket_fd         (0),
    m_url               (url),
    m_encryptor         (nullptr)
//    , m_acceptor          (nullptr)
{}


void CSocket::open() {
    LOGT;
   auto protocol    = IPPROTO_TCP;
    if (m_url.getProtocol() == URL::TProtocol::UDP)
        protocol    = IPPROTO_UDP;

    m_socket_fd     = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket open error");
//    LOGT << m_url << " " << static_cast<int>(m_socket_fd);
}


void CSocket::close() {
    LOGT << m_url << " " << static_cast<int>(m_socket_fd);
    if (m_ssl)
        m_ssl.reset();
//    if (m_acceptor)
//        m_acceptor->remove(this);
    assertOK(::close(m_socket_fd), "close socket error");
}


size_t CSocket::write(TPacket const &packet) {
//    LOCK_SCOPE;
    LOGT << m_url << " " << static_cast<int>(m_socket_fd);
    if (m_ssl) {
        LOGT << "ssl write";
        m_ssl->write(packet);
    } else {
        auto buffer = static_cast<void const *>(packet.data());
        auto result = ::send(m_socket_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE, 0);
        LOGT << "result " << result;
        if (result == EAGAIN)
            return DEFAULT_SOCKET_BUFFER_SIZE; // ----->
        else
            return result; // ----->

//        size_t lpos = 0;
//        while (lpos < packet.size()) {
//            auto buffer = static_cast<void const *>(packet.data() + lpos);
//            auto rpos   = lpos + DEFAULT_SOCKET_BUFFER_SIZE;

//            if (rpos > packet.size())
//                rpos = packet.size();

//            auto result = ::send(m_socket_fd, buffer, rpos - lpos, 0);
//            if (result == EAGAIN && !m_is_blocking_mode)
//                return lpos; // ----->

//            lpos += assertOK(result, "socket write error, lpos " + convert<string>(lpos));
////            LOGT << "lpos " << lpos;
//        }
//        return lpos;
    }
    return packet.size(); // ----->
}


CSocket::TPacket CSocket::read() {
//    LOCK_SCOPE
//    LOGT << m_url << " " << m_socket_fd;
    TPacket result;
    if (m_ssl) {
//        LOGT << "read ssl";
//        while (result.empty())
        result = m_ssl->read(DEFAULT_SOCKET_BUFFER_SIZE); // ----->
//        LOGT << "read ssl result " << result.size();
    } else {
        char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
        auto received_size = assertOK(::recv(m_socket_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0), "socket read error");
        LOGT << "result " << received_size;
//        LOGT << m_url << " " << m_socket_fd << " received_size " << received_size;
        result = TPacket(buffer, buffer + received_size); // ----->
    }
    return result; // ----->
}


void CSocket::listen() {
    LOGT;
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

    if (!m_encryptor && m_url.getProtocol() == URL::TProtocol::HTTPS)
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


// todo: async socket emulator, read and write by blocks 512 bytes for every open socket
ISocket::TEvents CSocket::accept() {
    throw std::runtime_error("not implemented"); // ----->
}


void CSocket::interrupt() {
    ::shutdown(m_socket_fd, 2);
}


void CSocket::connect() {
    LOGT;
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
//    LOCK_SCOPE;
    return m_url;
}


int CSocket::assertOK(int const &result, std::string const &message) const {
    if (result < 0)
        throw std::runtime_error(message + ": url " + convert<string>(m_url) + ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
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
        protocol    = convert<string>(m_url.getProtocol());
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


CSocket::TEvents CSocket::acceptInternal(std::list<int> &accepted_sockets_fd) {
    ISocket::TEvents events;

    struct sockaddr_in  client_address      = { 0 };
    socklen_t           client_address_size = sizeof(client_address);
    int                 socket_fd           = 0;

    do {
        socket_fd = ::accept(m_socket_fd, (struct sockaddr *) (&client_address), &client_address_size);
        if  (socket_fd > 0) {
            auto const url = getPeerURL(socket_fd);
            auto socket    = create(url);
            socket->m_socket_fd = socket_fd;

            if (m_encryptor)
                socket->m_ssl   = m_encryptor->accept(m_socket_fd);

            {
                auto event      = TEvent::create();
                event->action   = TEvent::TAction::ACCEPT;
                event->socket   = socket;

                events.push_back(event);
            }

//            {
//                auto event      = TEvent::create();
//                event->action   = TEvent::TAction::READ;
//                event->socket   = socket;

//                events.push_back(event);
//            }

            accepted_sockets_fd.push_back(socket_fd);
        }
    } while (socket_fd > 0);

    return events; // ----->
}


//CSocket::TSharedPtr CSocket::findAcceptedSocket(int const &socket_fd) {
//    LOCK_SCOPE;
//    auto const url = getPeerURL(socket_fd);
//    for (auto const &i: m_accepted_sockets)
//        if (i->getURL() == url)
//            return i; // ----->
//    return nullptr; // ----->
//}


//void CSocket::remove(CSocket const * const accepted_socket) {
//    auto url = assertExists(accepted_socket, "removing accepted socket is null")->getURL();
//    LOCK_SCOPE;
//    for (auto const &i: m_accepted_sockets) {
//        if (i->getURL() == url) {
////            LOGT << url << " " << i->m_socket_fd;
//            m_accepted_sockets.remove(i);
//            return; // ----->
//        }
//    }
////    LOGT << "sockets size " << m_accepted_sockets.size();
//}


} // unix
} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // UNIX_PLATFORM
