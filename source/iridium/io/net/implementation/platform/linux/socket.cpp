#include "socket.h"

#include <linux/tls.h>
#include <netinet/tcp.h>
#include <string.h>

#include "iridium/logging/logger.h"
#include "iridium/assert.h"


#ifdef LINUX_PLATFORM
#ifdef BUILD_FLAG_OPENSSL


using iridium::encryption::OpenSSL;


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
//    if (initializeAsync()) {
//        return unix::CSocket::write(buffer); // ----->
//    } else {
//        return 0;
//    }

    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        if (initializeAsync()) {
            assertExists(buffer, "write error: buffer is null");
            assertExists(m_ssl,  "write error: ssl is null");
            LOGT << "ssl write";
            return OpenSSL::instance().write(m_ssl, buffer); // ----->
        } else
            return 0; // ----->
    } else
        return unix::CSocket::write(buffer); // ----->
}


Buffer::TSharedPtr CSocket::read(size_t const &size) {
//    if (initializeAsync()) {
//        return unix::CSocket::read(size); // ----->
//    } else {
//        return Buffer::create(); // ----->
//    }

    if (m_url.getProtocol() == URL::TProtocol::HTTPS) {
        if (initializeAsync()) {
            auto result = OpenSSL::instance().read(m_ssl, size);
            LOGT << "ssl read: " << *result;
            return result;
        } else
            return Buffer::create(); // ----->
    } else
        return unix::CSocket::read(size); // ----->
}


bool CSocket::initializeAsync() {
    switch (m_ssl_init_step) {
    case 0: {
        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        LOGT << "connect...";
//        auto result = ::connect(m_socket, (struct sockaddr *) &m_address, sizeof(m_address));
//        LOGT << "connect " << result;
//        if (result == -1)
//            return false; // ----->
//        else
            m_ssl_init_step++;
    }
    case 1: {
        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
        LOGT << "context...";
        m_context   = OpenSSL::instance().createContext();
        m_ssl       = OpenSSL::instance().createSSL(m_context, m_socket);
        OpenSSL::instance().setConnectState(m_ssl);
        LOGT << "context OK";
        m_ssl_init_step++;
    }
    case 2: {
        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        LOGT << "connect SSL...";

//        auto result = API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ;
////        while (
////            result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
////            result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
////        {
//            result =  API::instance().connectSSL(m_ssl);
//            LOGT << "connect SSL " << result;
////        }

//        if (result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
//            result == API::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
//            return false; // ----->
//        else
            m_ssl_init_step++;
    }
    case 3: {
        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
        LOGT << "handshake SSL...";

        auto result =  OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ;
        while (
            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
            result =  OpenSSL::instance().doHandshake(m_ssl);

        LOGT << "handshake SSL " << result;
        if (result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
            return false; // ----->
        else
            m_ssl_init_step++;
    }
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
