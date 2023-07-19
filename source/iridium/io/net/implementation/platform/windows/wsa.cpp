#include "wsa.h"


#ifdef WINDOWS_PLATFORM


#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


// https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms737593(v=vs.85).aspx
// https://www.ibm.com/support/knowledgecenter/en/ssw_i5_54/rzab6/xnonblock.htm
// http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5f.html
// todo: io interface and async io pipe to file

#include "iridium/assert.h"
#include "iridium/convertion/convert.h"

#include <vector>


using iridium::convertion::convert;
using iridium::assertExists;
using std::string;
using std::wstring;
using std::vector;


#include "iridium/logging/logger.h"
#include <iostream>
namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


URI::TIPv4 WSA::getIPv4ByName(std::string const &name) {
    struct addrinfo hints = { 0 }, *servinfo = nullptr;
    URI::TIPv4      ipv4;

    hints.ai_family     = AF_UNSPEC;    // use AF_INET6 to force IPv6
    hints.ai_socktype   = SOCK_STREAM;

    int error_code      = getaddrinfo(name.c_str(), "http", &hints, &servinfo);

    if (servinfo) {
        auto ipv4_value =
            static_cast<in_addr>(reinterpret_cast<struct sockaddr_in *>
            (servinfo->ai_addr)->sin_addr).s_addr;

        ipv4[0]     = ipv4_value &  0xFF;
        ipv4_value  = ipv4_value >> 8;
        ipv4[1]     = ipv4_value &  0xFF;
        ipv4_value  = ipv4_value >> 8;
        ipv4[2]     = ipv4_value &  0xFF;
        ipv4_value  = ipv4_value >> 8;
        ipv4[3]     = ipv4_value &  0xFF;

        freeaddrinfo(servinfo);
    };

    assertEQ(error_code, 0, "resolving host name '" + name + "' error");

    return ipv4; // ----->
}


Buffer WSA::read(SOCKET const &socket, size_t const &size) {
    LOGT << socket;
    int             received_size = 0;
    vector<char>    buffer(size);
    Buffer          result;

    //ZeroMemory(buffer, size);
    received_size = ::recv(socket, buffer.data(), static_cast<int>(size), 0);
    if (received_size > 0)
        result.insert(result.end(), buffer.begin(), buffer.end());
        /*result.insert(result.end(), buffer.data(), buffer.data() + received_size);*/
    if (received_size < 0)
        throw std::runtime_error("socket read error: " + getLastWSAErrorString()); // ----->

    return result; // ----->
}


size_t  WSA::write(SOCKET const &socket, Buffer const &packet) {
    LOGT << socket;
    return assertNE(::send(socket, static_cast<char const *>(static_cast<void const *>(packet.data())), 
        static_cast<int>(packet.size()), 0), SOCKET_ERROR, "socket write error");
}


void WSA::close(SOCKET const &socket) {
    LOGT << socket;
    assertEQ(::closesocket(socket), 0, "socket close error");
}


SOCKET WSA::connect(URI const &uri) {
    sockaddr_in address = { 0 };

    auto ipv4 = *uri.getIPv4();

    address.sin_family  = AF_INET;
    auto a = (ipv4[0] << 24) | (ipv4[1] << 16) | (ipv4[2] << 8) | ipv4[3];
    address.sin_addr    = *(struct in_addr *)&a;
    address.sin_port    = uri.getPort();

    //if (uri.getProtocol() && *m_uri.getProtocol() == URI::TProtocol::UDP)
    //    address.ai_protocol   = IPPROTO_UDP;
    //else
    //    address.ai_protocol   = IPPROTO_TCP;

    struct addrinfo *address_result = nullptr;
    // resolve the server address and port
    assertEQ(::getaddrinfo(nullptr, convert<string>(uri.getPort()).c_str(), reinterpret_cast<ADDRINFOA *>(&address), &address_result), 0, "socket getaddrinfo error");
    // create a SOCKET for connecting to server
    auto socket = assertNE(::socket(address_result->ai_family, address_result->ai_socktype, address_result->ai_protocol), INVALID_SOCKET, "socket error");

    LOGT << uri << " connect";
    assertNE(::connect(socket, (struct sockaddr *)&address, sizeof(address)), SOCKET_ERROR, "socket connect error:");

    return socket; // ----->
}


SOCKET WSA::listen(URI const &uri) {
    auto listen_socket  = assertNE(WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED), INVALID_SOCKET, "create listen socket error");

    SOCKADDR_IN address = { 0 };

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(uri.getPort());

    assertNE(::bind  (listen_socket, (PSOCKADDR)&address, sizeof(address)), SOCKET_ERROR, "wsa socket bind error");
    assertEQ(::listen(listen_socket, SOMAXCONN), 0, "socket listen error");

    auto accept_socket = assertNE(
        WSASocket(
            AF_INET, 
            SOCK_STREAM, 
            0, 
            nullptr, 
            0, 
            WSA_FLAG_OVERLAPPED), 
        INVALID_SOCKET, "create accept socket error");

    WSAOVERLAPPED   overlapped          = { 0 };
    char            output_buffer[128]  = { 0 };
    DWORD           receive_data_length = 64;
    DWORD           bytes_received      = 0;

    WSAEVENT        event               = assertNE(WSACreateEvent(), WSA_INVALID_EVENT, "create wsa event error");

    assertEQ(
        AcceptEx(
            listen_socket, 
            accept_socket,
            (PVOID)(output_buffer),
            0,
            sizeof(SOCKADDR_IN) + 16, // dwLocalAddressLength
            sizeof(SOCKADDR_IN) + 16, // dwRemoteAddressLength
            &bytes_received,
            &overlapped), 
        FALSE, "wsa accept error");

    LOGT << "accept ex output_buffer    " << output_buffer;
    LOGT << "accept ex bytes_received   " << bytes_received;

    assertEQ(WSAGetLastError(), static_cast<int>(ERROR_IO_PENDING), "wsa accept error");

    DWORD       events_count = 1;
    WSAEVENT    events[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
    //WSAEVENT    event   = assertNE(WSACreateEvent(), WSA_INVALID_EVENT, "create wsa event error");

    overlapped.hEvent   = event;
    events[0]           = event;
    
    DWORD bytes_transferred = 0;
    DWORD flags = 0;

    while (true) {
        LOGT << "WSAWaitForMultipleEvents begin";
        auto index = assertNE(
            WSAWaitForMultipleEvents(
                events_count, 
                events, 
                false, 
                5000,//WSA_INFINITE, 
                false),
            WSA_WAIT_FAILED, "wsa event wait error");

        LOGT << "wait, index = " << index;
        LOGT << "buffer = " << output_buffer;

        LOGT << "WSAWaitForMultipleEvents end";

        if ((index - WSA_WAIT_EVENT_0) == 0) {
            LOGT << "WSAGetOverlappedResult begin";
            assertNE(
                WSAGetOverlappedResult(
                    listen_socket, 
                    &overlapped, 
                    &bytes_transferred, 
                    false, 
                    &flags), 
                static_cast<BOOL>(false), "wsa get overlapped result error");
            LOGT << "WSAGetOverlappedResult end";
            LOGT << "socket connected";
        } else {
            LOGT << "not accepted";
        }
    }

    return accept_socket;

    //return socket;
}


SOCKET WSA::accept(SOCKET const &socket) {
    LOGT << socket;

    return socket;

    //if (AcceptEx(ListenSocket, AcceptSocket, (PVOID)AcceptBuffer, 0,
    //    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Bytes, &ListenOverlapped) == FALSE)
    //
    //    if (WSAGetLastError() != ERROR_IO_PENDING) {
    //        printf("AcceptEx() failed with error %d\n", WSAGetLastError());
    //        return;
    //    } else
    //        printf("AcceptEx() is OK!\n");
    //    // Process asynchronous AcceptEx, WSASend, WSARecv requests
    //    while (TRUE) {
    //        if ((Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED) {
    //            printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());
    //            return;
    //        } else
    //            printf("WSAWaitForMultipleEvents() is OK!\n");
    //        // If the event triggered was zero then a connection attempt was made
    //        // on our listening socket
    //
    //        if ((Index - WSA_WAIT_EVENT_0) == 0) {
    //            // Check the returns from the overlapped I/O operation on the listening socket
    //
    //            if (WSAGetOverlappedResult(ListenSocket, &(ListenOverlapped), &BytesTransferred, FALSE, &Flags) == FALSE) {
    //                printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
    //                return;
    //            } else
    //                printf("WSAGetOverlappedResult() is OK!\n");
    //            printf("Socket %d got connected...\n", AcceptSocket);
    //
    //            if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS) {
    //                printf("Too many connections - closing socket.\n");
    //                closesocket(AcceptSocket);
    //                continue;
    //            } else {
    //                // Create a socket information structure to associate with the accepted socket
    //                if ((SocketArray[EventTotal] = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR,
    //                    sizeof(SOCKET_INFORMATION))) == NULL) {
    //                    printf("GlobalAlloc() failed with error %d\n", GetLastError());
    //                    return;
    //                } else
    //                    printf("GlobalAlloc() for LPSOCKET_INFORMATION is OK!\n");
    //                // Fill in the details of our accepted socket
    //                SocketArray[EventTotal]->Socket = AcceptSocket;
    //                ZeroMemory(&(SocketArray[EventTotal]->Overlapped), sizeof(OVERLAPPED));
    //                SocketArray[EventTotal]->BytesSEND = 0;
    //                SocketArray[EventTotal]->BytesRECV = 0;
    //                SocketArray[EventTotal]->DataBuf.len = DATA_BUFSIZE;
    //                SocketArray[EventTotal]->DataBuf.buf = SocketArray[EventTotal]->Buffer;
    //                if ((SocketArray[EventTotal]->Overlapped.hEvent = EventArray[EventTotal] = WSACreateEvent())
    //                    == WSA_INVALID_EVENT) {
    //                    printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
    //                    return;
    //                } else
    //                    printf("WSACreateEvent() is OK!\n");
    //                // Post a WSARecv request to to begin receiving data on the socket
    //                if (WSARecv(SocketArray[EventTotal]->Socket, &(SocketArray[EventTotal]->DataBuf), 1, &RecvBytes, &Flags,
    //                    &(SocketArray[EventTotal]->Overlapped), NULL) == SOCKET_ERROR) {
    //                    if (WSAGetLastError() != ERROR_IO_PENDING) {
    //                        printf("WSARecv() failed with error %d\n", WSAGetLastError());
    //                        return;
    //                    }
    //                } else
    //                    printf("WSARecv() is OK!\n");
    //                EventTotal++;
    //            }
    //            // Make a new socket for accepting future connections and post another AcceptEx call
    //            if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
    //                WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
    //                printf("Failed to get a socket %d\n", WSAGetLastError());
    //                return;
    //            } else
    //                printf("WSASocket() is OK!\n");
    //            WSAResetEvent(EventArray[0]);
    //            ZeroMemory(&ListenOverlapped, sizeof(OVERLAPPED));
    //            ListenOverlapped.hEvent = EventArray[0];
    //            if (AcceptEx(ListenSocket, AcceptSocket, (PVOID)AcceptBuffer, 0,
    //                sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Bytes, &ListenOverlapped) == FALSE) {
    //                if (WSAGetLastError() != ERROR_IO_PENDING) {
    //                    printf("AcceptEx() failed with error %d\n", WSAGetLastError());
    //                    return;
    //                }
    //            } else
    //                printf("AcceptEx() is OK!\n");
    //            continue;
    //        }
    //        SI = SocketArray[Index - WSA_WAIT_EVENT_0];
    //        WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
    //        if (WSAGetOverlappedResult(SI->Socket, &(SI->Overlapped), &BytesTransferred, FALSE, &Flags) == FALSE) {
    //            printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
    //            return;
    //        } else
    //            printf("WSAGetOverlappedResult() is OK!\n");
    //        // First check to see if the peer has closed the connection and if so
    //        // then close the socket and cleanup the SOCKET_INFORMATION structure
    //        // associated with the socket
    //        if (BytesTransferred == 0) {
    //            printf("Closing socket %d\n", SI->Socket);
    //            if (closesocket(SI->Socket) == SOCKET_ERROR) {
    //                printf("closesocket() failed with error %d\n", WSAGetLastError());
    //            } else
    //                printf("closesocket() is OK!\n");
    //            GlobalFree(SI);
    //            WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
    //            // Cleanup SocketArray and EventArray by removing the socket event handle
    //            // and socket information structure if they are not at the end of the arrays
    //            if ((Index - WSA_WAIT_EVENT_0) + 1 != EventTotal)
    //                for (i = Index - WSA_WAIT_EVENT_0; i < EventTotal; i++) {
    //                    EventArray[i] = EventArray[i + 1];
    //                    SocketArray[i] = SocketArray[i + 1];
    //                }
    //            EventTotal--;
    //            continue;
    //        }
    //        // Check to see if the BytesRECV field equals zero. If this is so, then
    //        // this means a WSARecv call just completed so update the BytesRECV field
    //        // with the BytesTransferred value from the completed WSARecv() call
    //        if (SI->BytesRECV == 0) {
    //            SI->BytesRECV = BytesTransferred;
    //            SI->BytesSEND = 0;
    //        } else {
    //            SI->BytesSEND += BytesTransferred;
    //        }
    //        if (SI->BytesRECV > SI->BytesSEND) {
    //            // Post another WSASend() request
    //            // Since WSASend() is not guaranteed to send all of the bytes requested,
    //            // continue posting WSASend() calls until all received bytes are sent
    //            ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
    //            SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
    //            SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
    //            SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;
    //            if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR) {
    //                if (WSAGetLastError() != ERROR_IO_PENDING) {
    //                    printf("WSASend() failed with error %d\n", WSAGetLastError());
    //                    return;
    //                }
    //            } else
    //                printf("WSASend() is OK!\n");
    //        } else {
    //            SI->BytesRECV = 0;
    //            // Now that there are no more bytes to send post another WSARecv() request
    //            Flags = 0;
    //            ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
    //            SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
    //            SI->DataBuf.len = DATA_BUFSIZE;
    //            SI->DataBuf.buf = SI->Buffer;
    //            if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR) {
    //                if (WSAGetLastError() != ERROR_IO_PENDING) {
    //                    printf("WSARecv() failed with error %d\n", WSAGetLastError());
    //                    return;
    //                }
    //            } else
    //                printf("WSARecv() is OK!\n");
    //        }
    //    }
}


void WSA::shutdown(SOCKET const &socket) {
    LOGT << socket;
    assertEQ(::shutdown(socket, 2), 0, "socket interrupt error"); // ----->
}


void WSA::setBlockingMode(SOCKET const &socket, bool const &is_blocking) {
    LOGT << socket << " " << is_blocking;
    u_long  mode    = is_blocking;
    auto    result  = ioctlsocket(socket, FIONBIO, &mode);

    if (result != NO_ERROR)
        throw std::runtime_error("socket setting non-blocking mode error"); // ----->
}


WSA::WSA()
:
    m_wsa_data({ 0 })
{
    assertEQ(::WSAStartup(MAKEWORD(2, 2), &m_wsa_data), 0, "WSA startup error");
}


WSA::~WSA() {
    ::WSACleanup();
}


//SOCKET WSA::assertOK(SOCKET const &socket, std::string const &message) const {
//    if (socket == INVALID_SOCKET) {
//        auto error = getLastWSAErrorString();
//        closesocket(socket);
//        throw std::runtime_error(message + ": " + error); // ----->
//    } else
//        return std::move(socket); // ----->
//}
//
//
//WSAEVENT WSA::assertOK(WSAEVENT const &result, std::string const &message) const {
//    if (result == WSA_INVALID_EVENT)
//        throw std::runtime_error(message + ": " + getLastWSAErrorString()); // ----->
//    else
//        return result; // ----->
//}


string WSA::getLastWSAErrorString() const {
    wchar_t *s = nullptr;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        //static_cast<LPWSTR>(s),
        (LPWSTR)&s,
        0, nullptr
    );

    auto result = convert<string>(wstring(s));

    // remove windows end line symbols
    result.pop_back();
    result.pop_back();
    LocalFree(s);

    return result; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM
