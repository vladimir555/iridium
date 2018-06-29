#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#ifndef HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B
#define HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B


#include "utility/net/url.h"
#include "utility/net/socket.h"


#include <ws2tcpip.h>


namespace utility {
namespace net {
namespace implementation {
namespace platform {


class CSocket: public ISocket {
public:
    DEFINE_IMPLEMENTATION(CSocket)
    CSocket(URL const &url);
	CSocket(URL const &url, SOCKET const &m_socket);

	void    open() override;
	void    close() override;
	size_t  write(TPacket const &packet) override;
	TPacket read() override;
	void    listen() override;
	TEvents accept() override;
	void    interrupt() override;
	void    connect() override;
	URL     getURL() const override;

private:
    URL     m_url;
    SOCKET  m_socket = INVALID_SOCKET;
};


} // platform
} // implementation
} // net
} // utility


#endif // HEADER_SOCKET_95D177C4_558D_4614_9449_385F91986A6B


#endif // WINDOWS_PLATFORM
