#include "wsa.h"


#ifdef WINDOWS_PLATFORM


#include <windows.h>
#include <winsock2.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


// https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms737593(v=vs.85).aspx


#include "utility/assert.h"
#include "utility/convertion/convert.h"

#include <vector>


using utility::convertion::convert;
using utility::assertExists;
using std::string;
using std::wstring;
using std::vector;


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


TIPv4 WSA::getIPv4ByName(std::string const &name) {
    struct addrinfo hints, *servinfo;
    TIPv4           ipv4;

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;    // use AF_INET6 to force IPv6
    hints.ai_socktype   = SOCK_STREAM;

    int error_code     = getaddrinfo(name.c_str(), "http", &hints, &servinfo);

    if (servinfo) {
        auto ipv4_value =
            static_cast<in_addr>(reinterpret_cast<struct sockaddr_in *>
            (servinfo->ai_addr)->sin_addr).s_addr;

        ipv4.push_back(ipv4_value & 0xFF);
        ipv4_value = ipv4_value >> 8;
        ipv4.push_back(ipv4_value & 0xFF);
        ipv4_value = ipv4_value >> 8;
        ipv4.push_back(ipv4_value & 0xFF);
        ipv4_value = ipv4_value >> 8;
        ipv4.push_back(ipv4_value & 0xFF);

        freeaddrinfo(servinfo);
    };

    assertOK(error_code, "get ip by host name '" + name + "' error: " + getLastWSAErrorString());

    return ipv4; // ----->
}


TPacket WSA::read(SOCKET const &socket, size_t const &size) {
    int             received_size = 0;
    vector<char>    buffer(size);
    TPacket		    result;

    //ZeroMemory(buffer, size);
    received_size = ::recv(socket, buffer.data(), size, 0);
    if (received_size > 0)
        result.insert(result.end(), buffer.begin(), buffer.end());
        /*result.insert(result.end(), buffer.data(), buffer.data() + received_size);*/
    if (received_size < 0)
        throw std::runtime_error("socket read error: " + getLastWSAErrorString()); // ----->

    return result; // ----->
}


size_t  WSA::write(SOCKET const &socket, TPacket const &packet) {
    return assertOK(
        ::send(socket, static_cast<char const *>(static_cast<void const *>(packet.data())), packet.size(), 0), 
        "socket write error"); // ----->
}


void WSA::close(SOCKET const &socket) {
    assertOK(::closesocket(socket), "socket close error");
}


SOCKET WSA::connect(URL const &url) {
    sockaddr_in     address = { 0 };

    auto ipv4 = *url.getIPv4();

    address.sin_family = AF_INET;
    auto a = (ipv4[0] << 24) | (ipv4[1] << 16) | (ipv4[2] << 8) | ipv4[3];
    address.sin_addr = *(struct in_addr *)&a;
    address.sin_port = *url.getPort();

    //if (m_url.getProtocol() && *m_url.getProtocol() == URL::TProtocol::UDP)
    //    address.ai_protocol   = IPPROTO_UDP;
    //else
    //    address.ai_protocol   = IPPROTO_TCP;

    struct addrinfo *address_result = nullptr;
    // resolve the server address and port
    //assertOK(::getaddrinfo(nullptr, convert<string>(*m_url.getPort()).c_str(), &address, &address_result), "socket getaddrinfo error");
    // create a SOCKET for connecting to server
    auto socket = assertOK(::socket(address_result->ai_family, address_result->ai_socktype, address_result->ai_protocol), "socket error");

    auto error_code = ::connect(socket, (struct sockaddr *)&address, sizeof(address));
    if  (error_code == SOCKET_ERROR)
        throw std::runtime_error("socket connect error: " + getLastWSAErrorString()); // ----->

    return socket; // ----->
}


SOCKET WSA::listen(URL const &url) {
    struct addrinfo *address_result = nullptr;
    try {
        struct addrinfo address = { 0 };

        address.ai_family   = AF_INET;
        address.ai_socktype = SOCK_STREAM;
        address.ai_flags    = AI_PASSIVE;

        if (url.getProtocol() == URL::TProtocol::UDP)
            address.ai_protocol = IPPROTO_UDP;
        else
            address.ai_protocol = IPPROTO_TCP;

        // resolve the server address and port
        assertOK(::getaddrinfo(nullptr, convert<string>(*url.getPort()).c_str(), &address, &address_result), "socket getaddrinfo error");
        // create a SOCKET for connecting to server
        auto socket = assertOK(::socket(address_result->ai_family, address_result->ai_socktype, address_result->ai_protocol), "socket error");
        // setup the TCP listening socket
        assertOK(bind(socket, address_result->ai_addr, (int)address_result->ai_addrlen), "socket bind error");
        // free struct pointer
        ::freeaddrinfo(address_result);
        assertOK(::listen(socket, SOMAXCONN), "socket listen error");

        return socket; // ----->
    } catch (std::exception const &) {
        ::freeaddrinfo(address_result);
        throw; // ----->
    }
}


SOCKET WSA::accept(SOCKET const &socket) {
    return assertOK(::accept(socket, nullptr, nullptr), "socket accept error"); // ----->
}


void WSA::shutdown(SOCKET const &socket) {
    assertOK(::shutdown(socket, 2), "socket interrupt error"); // ----->
}


void WSA::setBlockingMode(SOCKET const &socket, bool const &is_blocking) {
    u_long  mode    = is_blocking;
    auto    result  = ioctlsocket(socket, FIONBIO, &mode);

    if (result != NO_ERROR)
        throw std::runtime_error("socket setting non-blocking mode error"); // ----->
}


WSA::WSA() {
    assertOK(::WSAStartup(MAKEWORD(2, 2), &m_wsa_data), "WSA startup error");
}


WSA::~WSA() {
    WSACleanup();
}


template<>
SOCKET WSA::assertOK(SOCKET const &socket, std::string const &message) {
    if (socket == INVALID_SOCKET) {
        auto error = getLastWSAErrorString();
        closesocket(socket);
        WSACleanup();
        throw std::runtime_error(message + ": " + error); // ----->
    } else
        return std::move(socket); // ----->
}


template<>
int WSA::assertOK(int const &result, std::string const &message) {
    if (result == 0)
        return std::move(result); // ----->
    else {
        auto error = getLastWSAErrorString();
        WSACleanup();
        throw std::runtime_error(message + ": " + error); // ----->
    }
}


string WSA::getLastWSAErrorString() {
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


} // platform
} // implementation
} // networking
} // utility


#endif // WINDOWS_PLATFORM
