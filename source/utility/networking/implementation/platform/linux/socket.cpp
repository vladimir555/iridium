#include "utility/platform.h"


#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)


#include "socket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


using std::string;
using utility::convertion::convert;


namespace {


static int const DEFAULT_SOCKET_LISTEN_QUEUE_SIZE   = 16;
static int const DEFAULT_SOCKET_BUFFER_SIZE         = 512;


int assertOK(int const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ", " + std::strerror(errno)); // ----->
    else
        return result; // ----->
}


} // unnamed


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url)
:
    m_url(url),
    m_socket(0),
    m_socket_listen_queue_size(DEFAULT_SOCKET_LISTEN_QUEUE_SIZE)
{
    std::cout << "socket constructor 1" << std::endl;
}


CSocket::CSocket(URL const &url, int const &socket)
:
    m_url(url),
    m_socket(socket),
    m_socket_listen_queue_size(DEFAULT_SOCKET_LISTEN_QUEUE_SIZE)
{
    std::cout << "socket constructor 2 " << m_socket << std::endl;
}


CSocket::~CSocket() {
    //interrupt();
    std::cout << "socket destructor " << m_socket << std::endl;
}


void CSocket::open() {
    m_socket = assertOK(::socket(AF_INET, SOCK_STREAM, 0),
        "socket open error: url " + convert<string>(m_url));
}


void CSocket::close() {
    assertOK(::close(m_socket), "close socket error: url " + convert<string>(m_url));
}


void CSocket::write(packet_t const &packet) {
    void const *buffer = static_cast<void const *>(packet.data());
    auto write_size = assertOK(::write(m_socket, buffer, packet.size()),
        "socket write error: url " + convert<string>(m_url));
}


CSocket::packet_t CSocket::read() {
    char    buffer[DEFAULT_SOCKET_BUFFER_SIZE];

    auto received_size = assertOK(::recv(m_socket, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0),
        "socket read error: url " + convert<string>(m_url));

    return packet_t(buffer, buffer + received_size); // ----->
}


void CSocket::listen() {
//    assertOK( fcntl( m_socket, F_SETFL, O_NONBLOCK, 1 ),
//        "socket set non blocking fail" );

    struct sockaddr_in server_address = { 0 };

    server_address.sin_family       = AF_INET;
    server_address.sin_addr.s_addr  = INADDR_ANY;
    server_address.sin_port         = htons(*m_url.getPort());

    assertOK(::bind(m_socket, (struct sockaddr *) (&server_address), sizeof(server_address)),
        "socket bind error: url " + convert<string>(m_url));

    // waiting for incoming packet
    assertOK(::listen(m_socket, m_socket_listen_queue_size),
        "socket listen error: url " + convert<string>(m_url));
}


ISocketStream::TSharedPtr CSocket::accept() {
    struct sockaddr_in client_address = { 0 };
    socklen_t client_address_size = sizeof(client_address);

    auto socket_accept = assertOK(::accept(m_socket, (struct sockaddr *) (&client_address), &client_address_size),
        "socket accept error: url " + convert<string>(m_url));

    return CSocket::create(m_url, socket_accept); // ----->
}


void CSocket::interrupt() {
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

    assertOK(::connect(m_socket, (struct sockaddr *) &address, sizeof(address)),
        "socket connect error: url " + convert<string>(m_url));
}


} // platform
} // implementation
} // networking
} // utility


#endif // LINUX_PLATFORM FREEBSD_PLATFORM MACOS_PLATFORM
