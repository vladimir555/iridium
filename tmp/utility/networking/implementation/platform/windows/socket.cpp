#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#include "socket.h"

#include "utility/convertion/convert.h"
#include "utility/assert.h"


#include <windows.h>
#include <winsock2.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


// https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms737593(v=vs.85).aspx


using utility::convertion::convert;
using utility::assertExists;
using std::string;
using std::wstring;


namespace {


int const DEFAULT_BUFFER_SIZE = 512;


template<typename T>
T assertOK(T const &&result, std::string const &message);


string getLastWSAErrorString() {
    wchar_t *s = NULL;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, 
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&s, 0, NULL);
    auto result = convert<string>(wstring(s));
    // remove windows end line symbols
    result.pop_back();
    result.pop_back();
    LocalFree(s);
    return result; // ----->
}


template<>
SOCKET assertOK(SOCKET const &&socket, std::string const &message) {
    if (socket == INVALID_SOCKET) {
        auto error = getLastWSAErrorString();
        closesocket(socket);
        WSACleanup();
        throw std::runtime_error(message + ": " + error); // ----->
    } else
        return std::move(socket); // ----->
}


template<>
int assertOK(int const &&result, std::string const &message) {
    if (result == 0)
        return std::move(result); // ----->
    else {
        auto error = getLastWSAErrorString();
        WSACleanup();
        throw std::runtime_error(message + ": " + error); // ----->
    }
}


}


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url, SOCKET const &socket)
:
    m_url   (url), 
    m_socket(socket)
{}


CSocket::CSocket(URL const &url)
:
    m_url(url) 
{}


void CSocket::write(packet_t const &packet) {
    assertOK(::send(m_socket, static_cast<char const *>(static_cast<void const *>(packet.data())), packet.size(), 0), "socket write error");
}


CSocket::packet_t CSocket::read() {
    int         received_size = 0;
    char        buffer[DEFAULT_BUFFER_SIZE];
    packet_t    result;

    ZeroMemory(buffer, DEFAULT_BUFFER_SIZE);
    received_size = ::recv(m_socket, buffer, DEFAULT_BUFFER_SIZE, 0);
    if (received_size > 0)
        result.insert(result.end(), buffer, buffer + received_size);
    if (received_size < 0)
        throw std::runtime_error("socket read error: " + getLastWSAErrorString()); // ----->

    return result; // ----->
}


void CSocket::open() {
    //WSADATA wsa_data;
    // initialize win socket
    assertOK(::WSAStartup(MAKEWORD(2, 2), &m_wsa_data), "socket startup error");
}


void CSocket::close() {
    assertOK(::closesocket(m_socket), "socket close error");
}


void CSocket::connect() {
    sockaddr_in     address ={ 0 };

    auto ipv4 = *m_url.getIPv4();

    address.sin_family      = AF_INET;
    address.sin_addr        = (ipv4[0] << 24) | (ipv4[1] << 16) | (ipv4[2] << 8) | ipv4[3];
    address.sin_port        = *m_url.getPort();

    //if (m_url.getProtocol() && *m_url.getProtocol() == URL::TProtocol::UDP)
    //    address.ai_protocol   = IPPROTO_UDP;
    //else
    //    address.ai_protocol   = IPPROTO_TCP;

    struct addrinfo *address_result = nullptr;
    // resolve the server address and port
    assertOK(::getaddrinfo(nullptr, convert<string>(*m_url.getPort()).c_str(), &address, &address_result), "socket getaddrinfo error");
    // create a SOCKET for connecting to server
    m_socket = assertOK(::socket(address_result->ai_family, address_result->ai_socktype, address_result->ai_protocol), "socket error");

    auto result = ::connect(m_socket, (struct sockaddr *)&address, sizeof(address));

    if (result == SOCKET_ERROR)
        throw std::runtime_error("socket connect error: " + getLastWSAErrorString()); // ----->
}


void CSocket::listen() {
    struct addrinfo *address_result = nullptr;
    try {
        struct addrinfo address = { 0 };

        address.ai_family     = AF_INET;
        address.ai_socktype   = SOCK_STREAM;
        address.ai_flags      = AI_PASSIVE;

        if (m_url.getProtocol() && *m_url.getProtocol() == URL::TProtocol::UDP)
            address.ai_protocol   = IPPROTO_UDP;
        else
            address.ai_protocol   = IPPROTO_TCP;

        // resolve the server address and port
        assertOK(::getaddrinfo(nullptr, convert<string>(*m_url.getPort()).c_str(), &address, &address_result), "socket getaddrinfo error");
        // create a SOCKET for connecting to server
        m_socket = assertOK(::socket(address_result->ai_family, address_result->ai_socktype, address_result->ai_protocol), "socket error");
        // setup the TCP listening socket
        assertOK(bind(m_socket, address_result->ai_addr, (int)address_result->ai_addrlen), "socket bind error");
        // free struct pointer
        ::freeaddrinfo(address_result);
    } catch (std::exception const &) {
        ::freeaddrinfo(address_result);
        throw; // --->
    }
    assertOK(::listen(m_socket, SOMAXCONN), "socket listen error");
}


ISocketStream::TSharedPtr CSocket::accept() {
    // accept a client socket
    return CSocket::create(m_url, assertOK(::accept(m_socket, nullptr, nullptr), "socket accept error")); // ----->
}


void CSocket::interrupt() {
    assertOK(::shutdown(m_socket, 2), "socket interrupt error");
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // WINDOWS_PLATFORM
