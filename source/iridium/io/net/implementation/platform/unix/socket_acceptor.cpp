#include "socket_acceptor.h"


#ifdef UNIX_PLATFORM


#include "socket_peer.h"


using iridium::encryption::OpenSSL;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocketAcceptor::CSocketAcceptor(URL const &url)
:
    CSocketBase     (url),
    m_context       (nullptr),
    m_ssl           (nullptr)
{}


void CSocketAcceptor::initialize() {
    open();

    if (m_url->getProtocol() == URL::TProtocol::HTTPS) {
        m_context   = OpenSSL::instance().createContext(true);

        OpenSSL::instance().configureContext(m_context,
            encryption::DEFAULT_FILE_NAME_PRIVATE_KEY,
            encryption::DEFAULT_FILE_NAME_CERTIFICATE);
    }

    listen();
}


void CSocketAcceptor::finalize() {
    if (m_context)
        OpenSSL::instance().releaseContext(m_context);

    m_context = nullptr;

    close();
}


URL CSocketAcceptor::getURL() const {
    return CSocketBase::getURL();
}


int CSocketAcceptor::getID() const {
    return m_socket; // ----->
}


ISocketStream::TSharedPtr CSocketAcceptor::accept() {
    auto fd =  CSocketBase::accept();
    if  (fd) {
        if (m_context) {
            auto ssl = OpenSSL::instance().createSSL(m_context, fd);
            OpenSSL::instance().acceptSSL(ssl, m_is_blocking);
            return CSocketPeer::create(fd, m_url->getProtocol(), ssl); // ----->
        } else
            return CSocketPeer::create(fd, m_url->getProtocol()); // ----->
    } else
        return nullptr; // ----->
}


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM
