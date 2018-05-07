#include "utility/platform.h"


#ifdef WINDOWS_PLATFORM


#include "socket.h"

#include "wsa.h"

#include "utility/threading/thread.h"


int const DEFAULT_BUFFER_SIZE = 512;


#include "utility/logging/logger.h"
namespace utility {
namespace networking {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url)
:
    m_url(url) 
{}


CSocket::CSocket(URL const &url, SOCKET const &socket)
:
    m_url   (url),
    m_socket(socket) {
}


size_t CSocket::write(TPacket const &packet) {
    return WSA::instance().write(m_socket, packet); // ----->
}


TPacket CSocket::read() {
    return WSA::instance().read(m_socket, DEFAULT_BUFFER_SIZE); // ----->
}


void CSocket::open() {
}


void CSocket::close() {
    return WSA::instance().close(m_socket); // ----->
}


void CSocket::connect() {
    m_socket = WSA::instance().connect(m_url);
}


URL CSocket::getURL() const {
	return m_url;
}


void CSocket::listen() {
    m_socket = WSA::instance().listen(m_url);
    WSA::instance().setBlockingMode(m_socket, false);
}


CSocket::TEvents CSocket::accept() {
    TEvents events;
    threading::sleep(1000);
    LOGT << "try accept";
    auto s = WSA::instance().accept(m_socket);
    LOGT << "s = " << s;
    //events.push_back(TEvent::create(TEvent::TAction::ACCEPT, CSocket::create(m_url, WSA::instance().accept(m_socket))));
    return events;
}


void CSocket::interrupt() {
    WSA::instance().shutdown(m_socket);
}


} // platform
} // implementation
} // networking
} // utility


#else
void dummy() {}
#endif // WINDOWS_PLATFORM
