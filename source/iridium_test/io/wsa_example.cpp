#include <gtest/gtest.h>


#include <iridium/platform.h>
#ifdef WINDOWS_PLATFORM

// Link to ws2_32.lib
#include <winsock2.h>
// Link to mswsock.lib
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>


#define PORT 55555
#define DATA_BUFSIZE 8192


typedef struct _SOCKET_INFORMATION {
    CHAR            Buffer[DATA_BUFSIZE];
    WSABUF          DataBuf;
    SOCKET          Socket;
    WSAOVERLAPPED   Overlapped;
    DWORD           BytesSEND;
    DWORD           BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;


//int main(int argc, char** argv)
TEST(net, wsa_example) {
    return;
    DWORD EventTotal = 0;
    WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
    LPSOCKET_INFORMATION SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
    CHAR AcceptBuffer[2 * (sizeof(SOCKADDR_IN) + 16)];
    WSAOVERLAPPED ListenOverlapped;
    DWORD Bytes;
    DWORD Index;
    DWORD Flags;
    DWORD BytesTransferred;
    LPSOCKET_INFORMATION SI;
    WSADATA wsaData;
    SOCKET ListenSocket, AcceptSocket;
    SOCKADDR_IN InternetAddr;
    DWORD RecvBytes, SendBytes;
    DWORD i;


    if ((WSAStartup((2, 2), &wsaData)) != 0) {
        printf("WSAStartup() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return;
    } else
        printf("WSAStartup() is OK!\n");

    // listening

    if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        printf("Failed to get a socket %d\n", WSAGetLastError());
        return;
    } else
        printf("WSASocket() is OK!\n");
    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(PORT);
    if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR) {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return;
    } else
        printf("bind() is OK!\n");
    if (listen(ListenSocket, 5)) {
        printf("listen() failed with error %d\n", WSAGetLastError());
        return;
    } else
        printf("listen() is OK! I am listening...\n");

    // accepting

    // Setup the listening socket for connections
    if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        printf("Failed to get a socket %d\n", WSAGetLastError());
        return;
    } else
        printf("WSASocket() is OK!\n");

    ZeroMemory(&ListenOverlapped, sizeof(OVERLAPPED));
    if ((EventArray[0] = ListenOverlapped.hEvent = WSACreateEvent()) == WSA_INVALID_EVENT) {
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        return;
    } else
        printf("WSACreateEvent() is OK!\n");

    EventTotal = 1;
    if (AcceptEx(ListenSocket, AcceptSocket, (PVOID)AcceptBuffer, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Bytes, &ListenOverlapped) == FALSE)

        if (WSAGetLastError() != ERROR_IO_PENDING) {
            printf("AcceptEx() failed with error %d\n", WSAGetLastError());
            return;
        } else
            printf("AcceptEx() is OK!\n");

        // Process asynchronous AcceptEx, WSASend, WSARecv requests
        while (TRUE) {
            if ((Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED) {
                printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());
                return;
            } else
                printf("WSAWaitForMultipleEvents() is OK!\n");

            // If the event triggered was zero then a connection attempt was made
            // on our listening socket

            if ((Index - WSA_WAIT_EVENT_0) == 0) {
                // Check the returns from the overlapped I/O operation on the listening socket

                if (WSAGetOverlappedResult(ListenSocket, &(ListenOverlapped), &BytesTransferred, FALSE, &Flags) == FALSE) {
                    printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
                    return;
                } else
                    printf("WSAGetOverlappedResult() is OK!\n");
                printf("Socket %d got connected...\n", AcceptSocket);

                if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS) {
                    printf("Too many connections - closing socket.\n");
                    closesocket(AcceptSocket);
                    continue;
                } else {

                    // Create a socket information structure to associate with the accepted socket
                    if ((SocketArray[EventTotal] = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR,
                        sizeof(SOCKET_INFORMATION))) == NULL) {
                        printf("GlobalAlloc() failed with error %d\n", GetLastError());
                        return;

                    } else
                        printf("GlobalAlloc() for LPSOCKET_INFORMATION is OK!\n");
                    // Fill in the details of our accepted socket
                    SocketArray[EventTotal]->Socket = AcceptSocket;
                    ZeroMemory(&(SocketArray[EventTotal]->Overlapped), sizeof(OVERLAPPED));
                    SocketArray[EventTotal]->BytesSEND = 0;
                    SocketArray[EventTotal]->BytesRECV = 0;
                    SocketArray[EventTotal]->DataBuf.len = DATA_BUFSIZE;
                    SocketArray[EventTotal]->DataBuf.buf = SocketArray[EventTotal]->Buffer;
                    if ((SocketArray[EventTotal]->Overlapped.hEvent = EventArray[EventTotal] = WSACreateEvent())
                        == WSA_INVALID_EVENT) {
                        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
                        return;
                    } else
                        printf("WSACreateEvent() is OK!\n");
                    // Post a WSARecv request to to begin receiving data on the socket
                    if (WSARecv(SocketArray[EventTotal]->Socket, &(SocketArray[EventTotal]->DataBuf), 1, &RecvBytes, &Flags,
                        &(SocketArray[EventTotal]->Overlapped), NULL) == SOCKET_ERROR) {
                        if (WSAGetLastError() != ERROR_IO_PENDING) {
                            printf("WSARecv() failed with error %d\n", WSAGetLastError());
                            return;
                        }
                    } else
                        printf("WSARecv() is OK!\n");
                    EventTotal++;
                }
                // Make a new socket for accepting future connections and post another AcceptEx call
                if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
                    WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
                    printf("Failed to get a socket %d\n", WSAGetLastError());
                    return;
                } else
                    printf("WSASocket() is OK!\n");
                WSAResetEvent(EventArray[0]);
                ZeroMemory(&ListenOverlapped, sizeof(OVERLAPPED));
                ListenOverlapped.hEvent = EventArray[0];
                if (AcceptEx(ListenSocket, AcceptSocket, (PVOID)AcceptBuffer, 0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Bytes, &ListenOverlapped) == FALSE) {
                    if (WSAGetLastError() != ERROR_IO_PENDING) {
                        printf("AcceptEx() failed with error %d\n", WSAGetLastError());
                        return;
                    }
                } else
                    printf("AcceptEx() is OK!\n");
                continue;
            }
            SI = SocketArray[Index - WSA_WAIT_EVENT_0];
            WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
            if (WSAGetOverlappedResult(SI->Socket, &(SI->Overlapped), &BytesTransferred, FALSE, &Flags) == FALSE) {
                printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
                return;
            } else
                printf("WSAGetOverlappedResult() is OK!\n");
            // First check to see if the peer has closed the connection and if so
            // then close the socket and cleanup the SOCKET_INFORMATION structure
            // associated with the socket
            if (BytesTransferred == 0) {
                printf("Closing socket %d\n", SI->Socket);
                if (closesocket(SI->Socket) == SOCKET_ERROR) {
                    printf("closesocket() failed with error %d\n", WSAGetLastError());
                } else
                    printf("closesocket() is OK!\n");
                GlobalFree(SI);
                WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
                // Cleanup SocketArray and EventArray by removing the socket event handle
                // and socket information structure if they are not at the end of the arrays
                if ((Index - WSA_WAIT_EVENT_0) + 1 != EventTotal)
                    for (i = Index - WSA_WAIT_EVENT_0; i < EventTotal; i++) {
                        EventArray[i] = EventArray[i + 1];
                        SocketArray[i] = SocketArray[i + 1];
                    }
                EventTotal--;
                continue;
            }
            // Check to see if the BytesRECV field equals zero. If this is so, then
            // this means a WSARecv call just completed so update the BytesRECV field
            // with the BytesTransferred value from the completed WSARecv() call
            if (SI->BytesRECV == 0) {
                SI->BytesRECV = BytesTransferred;
                SI->BytesSEND = 0;
            } else {
                SI->BytesSEND += BytesTransferred;
            }

            if (SI->BytesRECV > SI->BytesSEND) {
                // Post another WSASend() request
                // Since WSASend() is not guaranteed to send all of the bytes requested,
                // continue posting WSASend() calls until all received bytes are sent
                ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
                SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
                SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
                SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;
                if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR) {
                    if (WSAGetLastError() != ERROR_IO_PENDING) {
                        printf("WSASend() failed with error %d\n", WSAGetLastError());
                        return;
                    }
                } else
                    printf("WSASend() is OK!\n");
            } else {
                SI->BytesRECV = 0;
                // Now that there are no more bytes to send post another WSARecv() request
                Flags = 0;
                ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
                SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
                SI->DataBuf.len = DATA_BUFSIZE;
                SI->DataBuf.buf = SI->Buffer;

                if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR) {
                    if (WSAGetLastError() != ERROR_IO_PENDING) {
                        printf("WSARecv() failed with error %d\n", WSAGetLastError());
                        return;
                    }
                } else
                    printf("WSARecv() is OK!\n");
            }
        }
}


//#ifndef UNICODE
//#define UNICODE
//#endif
//
//#define WIN32_LEAN_AND_MEAN
//
//#include <winsock2.h>
//#include <Ws2tcpip.h>
//#include <stdio.h>
//
//// Link with ws2_32.lib
//#pragma comment(lib, "Ws2_32.lib")
//
//#define DATA_BUFSIZE 4096

//TEST(net, wsa_example2) {
//    return;
//    //-----------------------------------------
//    // Declare and initialize variables
//    WSADATA wsaData = { 0 };
//    int iResult = 0;
//    BOOL bResult = TRUE;
//
//    WSABUF DataBuf;
//    char buffer[DATA_BUFSIZE];
//
//    DWORD EventTotal = 0;
//    DWORD RecvBytes = 0;
//    DWORD Flags = 0;
//    DWORD BytesTransferred = 0;
//
//    WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
//    WSAOVERLAPPED AcceptOverlapped;
//    SOCKET ListenSocket = INVALID_SOCKET;
//    SOCKET AcceptSocket = INVALID_SOCKET;
//
//    DWORD Index;
//
//    //-----------------------------------------
//    // Initialize Winsock
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (iResult != 0) {
//        wprintf(L"WSAStartup failed: %d\n", iResult);
//        return;
//    }
//    //-----------------------------------------
//    // Create a listening socket bound to a local
//    // IP address and the port specified
//    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (ListenSocket == INVALID_SOCKET) {
//        wprintf(L"socket failed with error = %d\n", WSAGetLastError());
//        WSACleanup();
//        return;
//    }
//
//    u_short port = 55555;
//    char *ip;
//    sockaddr_in service;
//    service.sin_family = AF_INET;
//    service.sin_port = htons(port);
//    hostent *thisHost;
//
//    thisHost = gethostbyname("");
//    if (thisHost == NULL) {
//        wprintf(L"gethostbyname failed with error = %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return;
//    }
//
//    ip = inet_ntoa(*(struct in_addr *) *thisHost->h_addr_list);
//
//    service.sin_addr.s_addr = inet_addr(ip);
//
//    //-----------------------------------------
//    // Bind the listening socket to the local IP address
//    // and port number
//    iResult = bind(ListenSocket, (SOCKADDR *)& service, sizeof (SOCKADDR));
//    if (iResult != 0) {
//        wprintf(L"bind failed with error = %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return;
//    }
//    //-----------------------------------------
//    // Set the socket to listen for incoming
//    // connection requests
//    iResult = listen(ListenSocket, 1);
//    if (iResult != 0) {
//        wprintf(L"listen failed with error = %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return;
//    }
//    wprintf(L"Listening...\n");
//
//    //-----------------------------------------
//    // Accept and incoming connection request
//    AcceptSocket = accept(ListenSocket, NULL, NULL);
//    if (AcceptSocket == INVALID_SOCKET) {
//        wprintf(L"accept failed with error = %d\n", WSAGetLastError());
//        closesocket(ListenSocket);
//        WSACleanup();
//        return;
//    }
//    wprintf(L"Client Accepted...\n");
//
//    //-----------------------------------------
//    // Create an event handle and setup an overlapped structure.
//    EventArray[EventTotal] = WSACreateEvent();
//    if (EventArray[EventTotal] == WSA_INVALID_EVENT) {
//        wprintf(L"WSACreateEvent failed with error = %d\n", WSAGetLastError());
//        closesocket(AcceptSocket);
//        closesocket(ListenSocket);
//        WSACleanup();
//        return;
//    }
//
//    ZeroMemory(&AcceptOverlapped, sizeof (WSAOVERLAPPED));
//    AcceptOverlapped.hEvent = EventArray[EventTotal];
//
//    DataBuf.len = DATA_BUFSIZE;
//    DataBuf.buf = buffer;
//
//    EventTotal++;
//
//    //-----------------------------------------
//    // Call WSARecv to receive data into DataBuf on 
//    // the accepted socket in overlapped I/O mode
//    if (WSARecv(AcceptSocket, &DataBuf, 1, &RecvBytes, &Flags, &AcceptOverlapped, NULL) ==
//        SOCKET_ERROR) {
//        iResult = WSAGetLastError();
//        if (iResult != WSA_IO_PENDING)
//            wprintf(L"WSARecv failed with error = %d\n", iResult);
//    }
//    //-----------------------------------------
//    // Process overlapped receives on the socket
//    while (1) {
//
//        //-----------------------------------------
//        // Wait for the overlapped I/O call to complete
//        Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);
//
//        //-----------------------------------------
//        // Reset the signaled event
//        bResult = WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
//        if (bResult == FALSE) {
//            wprintf(L"WSAResetEvent failed with error = %d\n", WSAGetLastError());
//        }
//        //-----------------------------------------
//        // Determine the status of the overlapped event
//        bResult =
//            WSAGetOverlappedResult(AcceptSocket, &AcceptOverlapped, &BytesTransferred, FALSE,
//                &Flags);
//        if (bResult == FALSE) {
//            wprintf(L"WSAGetOverlappedResult failed with error = %d\n", WSAGetLastError());
//        }
//        //-----------------------------------------
//        // If the connection has been closed, close the accepted socket
//        if (BytesTransferred == 0) {
//            wprintf(L"Closing accept Socket %d\n", AcceptSocket);
//            closesocket(ListenSocket);
//            closesocket(AcceptSocket);
//            WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
//            WSACleanup();
//            return;
//        }
//        //-----------------------------------------
//        // If data has been received, echo the received data
//        // from DataBuf back to the client
//        iResult =
//            WSASend(AcceptSocket, &DataBuf, 1, &RecvBytes, Flags, &AcceptOverlapped, NULL);
//        if (iResult != 0) {
//            wprintf(L"WSASend failed with error = %d\n", WSAGetLastError());
//        }
//        //-----------------------------------------         
//        // Reset the changed flags and overlapped structure
//        Flags = 0;
//        ZeroMemory(&AcceptOverlapped, sizeof (WSAOVERLAPPED));
//
//        AcceptOverlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
//
//        //-----------------------------------------
//        // Reset the data buffer
//        DataBuf.len = DATA_BUFSIZE;
//        DataBuf.buf = buffer;
//    }
//
//    closesocket(ListenSocket);
//    closesocket(AcceptSocket);
//    WSACleanup();
//
//    return;
//}

#endif