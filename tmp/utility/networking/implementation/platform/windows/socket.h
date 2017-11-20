#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#ifndef HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B
#define HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B


#include "utility/networking/url.h"
#include "utility/networking/socket.h"


#include <ws2tcpip.h>


namespace utility {
namespace networking {
namespace implementation {
namespace platform {


class CSocket: public ISocket {
public:
    DEFINE_CREATE(CSocket)
    CSocket(URL const &url, SOCKET const &m_socket);
    CSocket(URL const &url);
   ~CSocket() = default;

    void write(packet_t const &packet);
    packet_t read();

    void open();
    void close();
    void connect();
    void listen();
    ISocketStream::TSharedPtr accept();
    void interrupt();

private:
    URL     m_url;
    SOCKET  m_socket = INVALID_SOCKET;
    WSADATA m_wsa_data;
};


} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B


#endif // WINDOWS_PLATFORM
