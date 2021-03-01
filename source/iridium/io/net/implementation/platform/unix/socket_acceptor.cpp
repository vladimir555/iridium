#include "socket_acceptor.h"


#ifdef UNIX_PLATFORM


#include "socket_peer.h"


using iridium::encryption::OpenSSL;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocketAcceptor::CSocketAcceptor(URL const &url, IListenerStreams::TSharedPtr const &listener)
:
    CSocketBase     (url, listener),
    m_context       (nullptr)
{}


void CSocketAcceptor::initialize() {
    open();

    if (m_url->getProtocol() == URL::TProtocol::HTTPS) {
        m_context = OpenSSL::instance().createContext(true);

        OpenSSL::instance().configureContext(m_context,
            encryption::DEFAULT_FILE_NAME_PRIVATE_KEY,
            encryption::DEFAULT_FILE_NAME_CERTIFICATE);
    } else
        m_context = nullptr;

    listen();

    if (m_listener && m_is_opened)
        m_listener->add(shared_from_this());
}


void CSocketAcceptor::finalize() {
    if (m_listener && m_is_opened)
        m_listener->del(shared_from_this());

    if (m_context) {
        OpenSSL::instance().releaseContext(m_context);
        m_context = nullptr;
    }

    close();
}


URL CSocketAcceptor::getURL() const {
    return CSocketBase::getURL();
}


int CSocketAcceptor::getID() const {
    return m_socket; // ----->
}


ISocketStream::TSharedPtr CSocketAcceptor::accept() {
    auto fd = CSocketBase::accept();
    if  (fd) {
        if (m_context) {
            auto ssl = OpenSSL::instance().createSSL(m_context, fd);
            OpenSSL::instance().acceptSSL(ssl, !static_cast<bool>(m_listener));
            return CSocketPeer::create(fd, m_url->getProtocol(), ssl, m_listener); // ----->
        } else
            return CSocketPeer::create(fd, m_url->getProtocol(), m_listener); // ----->
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
