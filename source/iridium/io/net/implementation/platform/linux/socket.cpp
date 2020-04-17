#include "socket.h"

#include <linux/tls.h>
#include <netinet/tcp.h>
#include <string.h>

#include "iridium/logging/logger.h"


#ifdef LINUX_PLATFORM
#ifdef BUILD_FLAG_OPENSSL


using iridium::encryption::implementation::openssl::API;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


CSocket::CSocket(URL const &url, bool const &is_server_mode)
:
    unix::CSocket   (url, is_server_mode),
    m_ssl_init_step (0)
{}


void CSocket::initialize() {
    unix::CSocket::initialize();
    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        m_context   = API::instance().createContext("cert/iridium.key", "cert/iridium.crt");
        m_ssl       = API::instance().createSSL(m_context, m_socket);
//        API::instance().connectSSL(m_ssl);
//        // todo: int ssl init steps on read/wite methods
        m_ssl_init_step = 1;
    }
}


void CSocket::finalize() {
    API::instance().releaseSSL(m_ssl);
    API::instance().releaseContext(m_context);
    unix::CSocket::finalize();
}


ISocket::TSharedPtr CSocket::accept() {
    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        API::instance().acceptSSL(m_ssl, m_is_blocking);
        return nullptr; // todo:
    } else {
        return unix::CSocket::accept();
    }
}


size_t CSocket::write(Buffer::TSharedPtr const &buffer) {
    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        if (m_ssl_init_step == 1) {
            if (API::instance().connectSSL(m_ssl) == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
                return 0;
            else
                m_ssl_init_step++;
        }

        if (m_ssl_init_step == 2) {
            if (API::instance().doHandshake(m_ssl) == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
                return 0;
            else
                m_ssl_init_step++;
        }

        return API::instance().write(m_ssl, buffer);
    } else
        return unix::CSocket::write(buffer);
}


Buffer::TSharedPtr CSocket::read(size_t const &size) {
    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        if (m_ssl_init_step == 1) {
            if (API::instance().connectSSL(m_ssl) == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
                return {};
            else
                m_ssl_init_step++;
        }

        if (m_ssl_init_step == 2) {
            if (API::instance().doHandshake(m_ssl) == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
                return {};
            else
                m_ssl_init_step++;
        }

        return API::instance().read(m_ssl, size);
    } else
        return unix::CSocket::read(size);
}


//bool CSocket::continueReadSSLConnect() {
//    switch (m_ssl_init_step) {
//    case 0:
//        m_context   = API::instance().createContext("cert/iridium.key", "cert/iridium.crt");
//        m_ssl       = API::instance().createSSL(m_context, m_socket);
//        break;
//    }
//    m_ssl_init_step++;
//    return true;
//}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // BUILD_FLAG_OPENSSL
#endif // LINUX_PLATFORM
