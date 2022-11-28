#include "socket_client.h"


#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM)


#include "iridium/items.h"


using iridium::encryption::OpenSSL;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix {


CSocketClient::CSocketClient(URL const &url, IListener::TSharedPtr const &listener)
:
    CSocketBase             (url, listener),
    m_ssl                   (nullptr),
    m_is_ssl_handshake_done (false)
{}


void CSocketClient::initialize() {
    LOGT << *m_url;
    open();
    connect();

    if (m_url->getProtocol() == URL::TProtocol::HTTPS) {
        m_context   = OpenSSL::instance().createContext(false);
        m_ssl       = OpenSSL::instance().createSSL(m_context, m_socket);
        auto code   = OpenSSL::instance().setConnectState(m_ssl);
        LOGT << "set_connect_state code = " << code;
        assertSSLInitialized();
        m_is_ssl_handshake_done = false;
    }

    if (m_listener && m_is_opened) {
        LOGT << "add to listener fd " << getID();
        m_listener->add(shared_from_this());
    }
}


void CSocketClient::finalize() {
    if (m_listener && m_is_opened)
        m_listener->del(shared_from_this());

    if (m_ssl) {
        OpenSSL::instance().releaseSSL(m_ssl);
        m_ssl = nullptr;
    }

    if (m_context) {
        OpenSSL::instance().releaseContext(m_context);
        m_context = nullptr;
    }

    close();
}


URL CSocketClient::getURL() const {
    return CSocketBase::getURL();
}


int CSocketClient::getID() const {
    return CSocketBase::getID(); // ----->
}


Buffer::TSharedPtr CSocketClient::read(size_t const &size) {
    if (m_ssl) {
        if (assertSSLInitialized())
            return OpenSSL::instance().read(m_ssl, size); // ----->
        else
            return Buffer::create(); // ----->
    } else
        return CSocketBase::read(size); // ----->
}


size_t CSocketClient::write(Buffer::TSharedPtr const &buffer) {
    if (m_ssl) {
        if (assertSSLInitialized())
            return OpenSSL::instance().write(m_ssl, buffer); // ----->
        else
            return 0; // ----->
    } else
        return CSocketBase::write(buffer); // ----->
}


bool CSocketClient::assertSSLInitialized() {
    if (m_is_ssl_handshake_done)
        return true; // ----->

    auto    code =  OpenSSL::instance().doHandshake(m_ssl);
    LOGT << "do_handshake code = " << code;
    m_is_ssl_handshake_done = !checkOneOf(code,
        OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ,
        OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE);

    return m_is_ssl_handshake_done; // ----->
}


} // unix
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // LINUX_PLATFORM || FREEBSD_PLATFORM
