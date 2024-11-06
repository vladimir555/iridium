#include "socket.h"


#include "iridium/logging/logger.h"
#include "iridium/assert.h"


#ifdef LINUX_PLATFORM


//#include <linux/tls.h>
#include <netinet/tcp.h>
#include <string.h>


#ifdef BUILD_FLAG_OPENSSL


#include "iridium/encryption/keys.h"


using iridium::encryption::OpenSSL;


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {

//// todo:
//// CSocketAcceptor
//// CSocketClient
//// CSocketPeer
//CSocket::CSocket(URI const &uri, TMode const &mode)
//:
//    unix::CSocket   (uri, mode),
//    m_ssl           (nullptr),
//    m_ssl_init_step (0)
//{}


//CSocket::CSocket(URI const &uri, int const &fd, encryption::OpenSSL::TSSL *ssl)
//:
//    unix::CSocket   (uri, fd),
//    m_ssl_init_step (4)
//{
//    m_mode  = TMode::PEER;
//    m_ssl   = ssl;
//}


//void CSocket::initialize() {
//    if (m_mode != TMode::PEER)
//        unix::CSocket::initialize();

//    if (m_uri.getProtocol() == URI::TProtocol::HTTPS && m_mode == TMode::SERVER) {
//        m_context   = OpenSSL::instance().createContext(true);

//        OpenSSL::instance().configureContext(m_context,
//            encryption::DEFAULT_FILE_NAME_PRIVATE_KEY,
//            encryption::DEFAULT_FILE_NAME_CERTIFICATE);
//        m_ssl_init_step = 4;
//    }
//}


//void CSocket::finalize() {
//    if (m_uri.getProtocol() == URI::TProtocol::HTTPS) {
//        if (m_ssl)
//            OpenSSL::instance().releaseSSL(m_ssl);
//        OpenSSL::instance().releaseContext(m_context);
//    }
//    unix::CSocket::finalize();
//}


//ISocket::TSharedPtr CSocket::accept() {
//    if (m_uri.getProtocol() == URI::TProtocol::HTTPS) {
//        auto socket = unix::CSocket::accept();
//        if (!socket)
//            return nullptr; // ----->
//        auto ssl    = OpenSSL::instance().createSSL(m_context, socket->getID());
//        LOGT << "! " << m_is_blocking;
//        OpenSSL::instance().acceptSSL(ssl, m_is_blocking);

//        return CSocket::create(socket->getURI(), socket->getID(), ssl);
//    } else {
//        return unix::CSocket::accept();
//    }
//    return nullptr;
//}


//size_t CSocket::write(Buffer::TSharedPtr const &buffer) {
//    if (m_uri.getProtocol() == URI::TProtocol::HTTPS) {
//        if (initializeSSLAsync()) {
//            assertExists(buffer, "write error: buffer is null");
//            assertExists(m_ssl,  "write error: ssl is null");
//            LOGT << "ssl write";
//            return OpenSSL::instance().write(m_ssl, buffer); // ----->
//        } else
//            return 0; // ----->
//    } else
//        return unix::CSocket::write(buffer); // ----->
//}


//Buffer::TSharedPtr CSocket::read(size_t const &size) {
//    if (m_uri.getProtocol() == URI::TProtocol::HTTPS) {
//        if (initializeSSLAsync()) {
//            auto result = OpenSSL::instance().read(m_ssl, size);
//            LOGT << "ssl read: " << *result;
//            return result;
//        } else
//            return Buffer::create(); // ----->
//    } else
//        return unix::CSocket::read(size); // ----->
//}


//bool CSocket::initializeSSLAsync() {
//    switch (m_ssl_init_step) {
//    case 0: {
//        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        m_ssl_init_step++;
//    }
//    case 1: {
//        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        LOGT << "context...";

//        if (m_mode == TMode::CLIENT) {
//            m_context   = OpenSSL::instance().createContext();
//            m_ssl       = OpenSSL::instance().createSSL(m_context, m_socket);
//            OpenSSL::instance().setConnectState(m_ssl);
//        }


//        LOGT << "context OK";
//        m_ssl_init_step++;
//    }
//    case 2: {
//        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        m_ssl_init_step++;
//    }
//    case 3: {
//        LOGT << "-----m_ssl_init_step = " << m_ssl_init_step;
//        LOGT << "handshake SSL...";

//        auto result =  OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ;
//        while (
//            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
//            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
//            result =  OpenSSL::instance().doHandshake(m_ssl);

//        LOGT << "handshake SSL " << result;
//        if (result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
//            result == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
//            return false; // ----->
//        else
//            m_ssl_init_step++;
//    }
//    default:
//        LOGT << "skip";
//        return true; // ----->
//    }
//    LOGT << "done";
//    return true; // ----->
//}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // BUILD_FLAG_OPENSSL
#endif // LINUX_PLATFORM
