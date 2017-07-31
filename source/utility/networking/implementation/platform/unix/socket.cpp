#include "utility/platform.h"



#ifdef UNIX_PLATFORM


#include "socket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "utility/logging/logger.h"

using std::string;
using utility::convertion::convert;


namespace {


//static int const DEFAULT_SOCKET_LISTEN_QUEUE_SIZE   = SOMAXCONN;
static int const DEFAULT_SOCKET_BUFFER_SIZE         = 512;


} // unnamed


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url, bool const &is_blocking_connection)
:
    m_url(url),
    m_socket(0),
    m_is_blocking_connection(is_blocking_connection)
{
    LOGT << "url " << url << " is_blocking_connection " << is_blocking_connection;
}


CSocket::CSocket(URL const &url, int const &socket)
:
    m_url(url),
    m_socket(socket)
{
    LOGT << "url " << url << " socket ";
}


CSocket::~CSocket() {
    //interrupt();
    LOGT << "destructor";
}


void CSocket::open() {
    // todo: SOCK_STREAM - tcp, SOCK_DGRAM - udp

    LOGT << "::socket AF_INET, SOCK_STREAM ...";

    m_socket = assertOK(::socket(AF_INET, SOCK_STREAM, 0),
        "socket open error");

    LOGT << "::socket socket " << m_socket;

    if (m_is_blocking_connection) {
        // todo:
    } else {
//        assertOK( fcntl( m_socket, F_SETFL, O_NONBLOCK, 1 ),
//            "socket set non blocking fail" );

//        LOGT << "::fcntl socket " << m_socket;
//        assertOK( fcntl( m_socket, F_SETFL, fcntl( m_socket, F_GETFL, 0 ) | O_NONBLOCK ),
//              "socket set non blocking fail" );
    }
}


void CSocket::close() {
    LOGT << "::close socket " << m_socket;
    assertOK(::close(m_socket), "close socket error");
}


void CSocket::write(packet_t const &packet) {
    LOGT << "::write socket " << m_socket << " packet " << packet << " ...";
    size_t lpos = 0;
    while (lpos < packet.size()) {
        auto buffer = static_cast<void const *>(packet.data() + lpos);
        auto rpos = lpos + DEFAULT_SOCKET_BUFFER_SIZE;
        if (rpos > packet.size())
            rpos = packet.size();
        lpos += assertOK(::send(m_socket, buffer, rpos - lpos, 0),
            "socket write error");
    }
    LOGT << "::write write_size " << lpos;
}


CSocket::packet_t CSocket::read() {
    char buffer[DEFAULT_SOCKET_BUFFER_SIZE];

    LOGT << "::recv socket " << m_socket << "...";

    auto received_size = assertOK(::recv(m_socket, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0),
        "socket read error");

    auto result = packet_t(buffer, buffer + received_size);

    LOGT << "::recv socket " << m_socket << " received_size " << received_size << " buffer\n" << result;
    return result; // ----->
}


void CSocket::listen() {
    //todo: unix / inet protocol by url
    struct sockaddr_in server_address = { 0 };

    server_address.sin_family       = AF_INET;
    server_address.sin_addr.s_addr  = INADDR_ANY;
    server_address.sin_port         = htons(*m_url.getPort());

    LOGT << "::bind socket " << m_socket;
    assertOK(::bind(m_socket, (struct sockaddr *) (&server_address), sizeof(server_address)),
        "socket bind error");

    // waiting for incoming packet
    LOGT << "::listen socket " << m_socket;
    assertOK(::listen(m_socket, SOMAXCONN),
        "socket listen error");
}


ISocketStream::TSharedPtr CSocket::accept() {
    struct sockaddr_in client_address = { 0 };
    socklen_t client_address_size = sizeof(client_address);

    LOGT << "::accept socket " << m_socket << "... ";

    auto socket_accept = assertOK(::accept(m_socket, (struct sockaddr *) (&client_address), &client_address_size),
        "socket accept error");

    LOGT << "::accept socket " << m_socket << " accepted " << socket_accept;

    return ISocketStream::TSharedPtr(new CSocket(m_url, socket_accept)); // ----->
}


void CSocket::interrupt() {
    LOGT << "none";
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


} // platform
} // implementation
} // networking
} // utility


#endif // UNIX_PLATFORM
