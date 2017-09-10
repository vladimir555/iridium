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

#include "utility/logging/logger.h"


using std::string;
using utility::convertion::convert;


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
    m_url(url),
    m_socket(0)
{
    LOGT << "url " << url;
}


CSocket::CSocket(URL const &url, int const &socket)
:
    m_url(url),
    m_socket(socket)
{
    LOGT << "url " << url << " socket " << socket;
}


CSocket::~CSocket() {
    //interrupt();
    LOGT << "socket " << m_socket;
}


void CSocket::open() {
    // todo: SOCK_STREAM - tcp, SOCK_DGRAM - udp

    LOGT << "::socket AF_INET, SOCK_STREAM ...";

    auto protocol = IPPROTO_IP;
    if (!m_url.getProtocol() || *m_url.getProtocol() == URL::TProtocol::UDP)
        protocol = IPPROTO_UDP;
    else
        protocol = IPPROTO_TCP;

    m_socket = assertOK(::socket(AF_INET, SOCK_STREAM, protocol),
        "socket open error");

    LOGT << "socket " << m_socket;
}


void CSocket::close() {
    LOGT << "::close socket " << m_socket;
    assertOK(::close(m_socket), "close socket error");
}


void CSocket::write(TPacket const &packet) {
//    LOGT << "::write socket " << m_socket << " packet " << packet;
    size_t lpos = 0;
    while (lpos < packet.size()) {
        auto buffer = static_cast<void const *>(packet.data() + lpos);
        auto rpos = lpos + DEFAULT_SOCKET_BUFFER_SIZE;
        if (rpos > packet.size())
            rpos = packet.size();
        lpos += assertOK(::send(m_socket, buffer, rpos - lpos, 0),
            "socket write error");
    }
    LOGT << "::write socket " << m_socket << " write_size " << lpos;
}


CSocket::TPacket CSocket::read() {
    char buffer[DEFAULT_SOCKET_BUFFER_SIZE];

//    LOGT << "::recv socket " << m_socket << "...";

    auto received_size = assertOK(::recv(m_socket, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0),
        "socket read error");

    auto result = TPacket(buffer, buffer + received_size);

    LOGT << "::recv socket " << m_socket << " received_size " << received_size;// << " buffer\n" << result;
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
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    LOGT << "::bind socket " << m_socket;
    assertOK(::bind(m_socket, (struct sockaddr *) (&server_address), sizeof(server_address)),
        "socket bind error");

    // waiting for incoming packet
    LOGT << "::listen socket " << m_socket;
    assertOK(::listen(m_socket, SOMAXCONN),
        "socket listen error");
}


ISocket::TSocketStreams CSocket::accept() {
    struct sockaddr_in client_address = { 0 };
    socklen_t client_address_size = sizeof(client_address);

    LOGT << "::accept socket " << m_socket << "... ";

    auto client_socket = assertOK(::accept(m_socket, (struct sockaddr *) (&client_address), &client_address_size),
        "socket accept error");

    LOGT << "::accept socket " << m_socket << " accepted " << client_socket;

    return ISocket::TSocketStreams(
        { ISocketStream::TSharedPtr(new unix::CSocket(getPeerURL(client_socket), client_socket)) }
    ); // ----->
}


void CSocket::interrupt() {
    LOGT << "::shutdown socket " << m_socket;
    ::shutdown(m_socket, 2);
//    assertOK(::shutdown(m_socket, 2),
//        "socket interrupt error: url " + convert<string>(m_url));
}


void CSocket::connect() {
    struct sockaddr_in address;
    auto ipv4 = *m_url.getIPv4();
    address.sin_addr.s_addr  = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    address.sin_family       = AF_INET;
    address.sin_port         = htons( *m_url.getPort() );

    LOGT << "::connect socket " << m_socket;

    assertOK(::connect(m_socket, (struct sockaddr *) &address, sizeof(address)),
        "socket connect error");
}


int CSocket::assertOK(int const &result, std::string const &message) const {
    if (result < 0)
        throw std::runtime_error(message + ": url " + convert<string>(m_url) +
            ", " + std::strerror(errno)); // ----->
    else
        return result; // ----->
}


URL CSocket::getPeerURL(int const &socket) {
    struct sockaddr_in   peer;
    unsigned int         peer_len = sizeof(peer);
    assertOK(::getpeername(socket, (sockaddr *)&peer, &peer_len),
        "socket get peer ip error");
    return URL(string("tcp://") + inet_ntoa(peer.sin_addr) + ":" + convert<string>(ntohs(peer.sin_port)));
}


} // unix
} // platform
} // implementation
} // networking
} // utility


#endif // UNIX_PLATFORM
