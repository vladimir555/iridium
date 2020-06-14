#include "socket.h"

#include <linux/tls.h>
#include <netinet/tcp.h>
#include <string.h>

#include "iridium/logging/logger.h"
#include "iridium/assert.h"


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

//    assertOK(::connect(m_socket, (struct sockaddr *) &m_address, sizeof(m_address)), "socket connect error", m_url);

//    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
//        m_context   = API::instance().createContext("cert/iridium.key", "cert/iridium.crt");
//        m_ssl       = API::instance().createSSL(m_context, m_socket);
////        API::instance().connectSSL(m_ssl);
////        // todo: int ssl init steps on read/wite methods
//        m_ssl_init_step = 1;
//    }

}


void CSocket::finalize() {
//    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
//        API::instance().releaseSSL(m_ssl);
//        API::instance().releaseContext(m_context);
//    }
    unix::CSocket::finalize();
}


ISocket::TSharedPtr CSocket::accept() {
//    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
//        API::instance().acceptSSL(m_ssl, m_is_blocking);
//        return nullptr; // todo:
//    } else {
        return unix::CSocket::accept();
//    }
}


size_t CSocket::write(Buffer::TSharedPtr const &buffer) {
    if (initializeAsync()) {
        return unix::CSocket::write(buffer); // ----->
    } else {
        return 0;
    }

//    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
//        if (initializeAsync()) {
//            assertExists(buffer, "write error: buffer is null");
//            assertExists(m_ssl, "write error: ssl is null");
//            return API::instance().write(m_ssl, buffer); // ----->
//        } else
//            return 0; // ----->
//    } else
//        return unix::CSocket::write(buffer); // ----->
}


Buffer::TSharedPtr CSocket::read(size_t const &size) {
    if (initializeAsync()) {
        return unix::CSocket::read(size); // ----->
    } else {
        return Buffer::create(); // ----->
    }

//    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
//        if (initializeAsync()) {
//            auto result = API::instance().read(m_ssl, size);
//            LOGT << "ssl read: " << *result;
//            return result;
//        } else
//            return Buffer::create(); // ----->
//    } else
//        return unix::CSocket::read(size); // ----->
}


bool CSocket::initializeAsync() {
    LOGT << "m_ssl_init_step = " << m_ssl_init_step;
    switch (m_ssl_init_step) {
    case 0: {
        LOGT << "connect...";
        auto result = ::connect(m_socket, (struct sockaddr *) &m_address, sizeof(m_address));
        LOGT << "connect " << result;
        if (result == -1)
            return false; // ----->
        else
            m_ssl_init_step++;
    }
//    case 1: {
//        LOGT << "context...";
//        m_context   = API::instance().createContext();
//        m_ssl       = API::instance().createSSL(m_context, m_socket);
//        LOGT << "context OK";
//        m_ssl_init_step++;
//    }
//    case 2: {
//        LOGT << "connect SSL...";
//        auto result = API::instance().connectSSL(m_ssl);
//        LOGT << "connect SSL " << result;
//        if (result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
//            return false; // ----->
//        else
//            m_ssl_init_step++;
//    }
//    case 3: {
//        LOGT << "handshake SSL...";
//        auto result = API::instance().doHandshake(m_ssl);
//        LOGT << "handshake SSL " << result;
//        if (result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ)
//            return false; // ----->
//        else
//            m_ssl_init_step++;
//    }
    default:
        LOGT << "skip";
        return true; // ----->
    }
    LOGT << "done";
    return true; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // BUILD_FLAG_OPENSSL
#endif // LINUX_PLATFORM
