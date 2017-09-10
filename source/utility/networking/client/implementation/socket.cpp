#include "socket.h"



#include "utility/networking/implementation/socket.h"


namespace utility {
namespace networking {
namespace client {
namespace implementation {


CSocket::CSocket(URL const &url) {
    m_socket = networking::implementation::CSocket::create(url);
}


void CSocket::initialize() {
    m_socket->open();
    m_socket->connect();
}


void CSocket::finalize() {
    m_socket->close();
}


void CSocket::write(TPacket const &packet) {
    m_socket->write(packet);
}


CSocket::TPacket CSocket::read() {
    return m_socket->read();
}


} // implementation
} // client
} // networking
} // utility
