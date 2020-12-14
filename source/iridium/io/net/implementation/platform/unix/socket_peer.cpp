#include "socket_peer.h"


#ifdef UNIX_PLATFORM


using iridium::encryption::OpenSSL;
using iridium::convertion::convert;
using std::string;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocketPeer::CSocketPeer(int const &fd, URL::TProtocol const &protocol)
:
    CSocketBase (fd),
    m_protocol  (protocol),
    m_ssl       (nullptr)
{}


CSocketPeer::CSocketPeer(int const &fd, URL::TProtocol const &protocol, encryption::OpenSSL::TSSL *ssl)
:
    CSocketBase (fd),
    m_protocol  (protocol),
    m_ssl       (ssl)
{}


void CSocketPeer::initialize() {
    m_url = URL::create(getSocketURL(m_protocol));
}


void CSocketPeer::finalize() {
    close();
}


URL CSocketPeer::getURL() const {
    return CSocketBase::getURL();
}


int CSocketPeer::getID() const {
    return m_socket; // ----->
}


Buffer::TSharedPtr CSocketPeer::read(size_t const &size) {
    if (m_ssl)
        return OpenSSL::instance().read(m_ssl, size); // ----->
    else
        return CSocketBase::read(size); // ----->
}


size_t CSocketPeer::write(Buffer::TSharedPtr const &buffer) {
    if (m_ssl)
        return OpenSSL::instance().write(m_ssl, buffer); // ----->
    else
        return CSocketBase::write(buffer); // ----->
}


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
