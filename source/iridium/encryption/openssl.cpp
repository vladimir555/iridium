#include "openssl.h"


#ifdef BUILD_FLAG_OPENSSL


#include "iridium/logging/logger.h"
#include "iridium/assert.h"

#include "iridium/threading/thread.h"
#include "iridium/items.h"
//#include <signal.h>


using std::string;
using iridium::convertion::convert;


IMPLEMENT_ENUM(iridium::encryption::OpenSSL::TErrorCode)
IMPLEMENT_ENUM(iridium::encryption::OpenSSL::TSSLErrorCode)


//// todo: signals handlers singleton
//void handleSignal(int signal) {
//    LOGT << "broken pipe signal " << signal;
//}


namespace iridium {
namespace encryption {
//namespace implementation {
//namespace openssl {


OpenSSL::OpenSSL() {
    LOGT << "initialize openssl";
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();

//    LOGT << "set empty signal handler for broken ssl pipe";
//    struct sigaction sh;
//    struct sigaction osh;

//    // can set to SIG_IGN
//    sh.sa_handler   = &handleSignal;
//    // restart interrupted system calls
//    sh.sa_flags     = SA_RESTART;

//    // block every signal during the handler
//    sigemptyset(&sh.sa_mask);

//    if (sigaction(SIGPIPE, &sh, &osh) < 0)
//        throw std::runtime_error("sigaction error");
}


OpenSSL::~OpenSSL() {
    ERR_free_strings();
    EVP_cleanup();
}


string OpenSSL::getErrorString() {
    BIO    *bio     = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char   *buffer  = nullptr;
    size_t  length  = BIO_get_mem_data(bio, &buffer);
    return  string(buffer, length); // ----->
}


string OpenSSL::getSSLErrorString(TSSL *ssl, int const &code) {
    auto result = SSL_get_error(ssl, code);
    if  (result == TSSLErrorCode::SSL_ERROR_CODE_NONE)
        return std::string(ERR_error_string(result, nullptr)) +
            ", error code: " + convert<std::string>(static_cast<TSSLErrorCode>(code)) +
            ", " + string(ERR_error_string(ERR_get_error(), nullptr)); // ----->
    else
        return "SSL_get_error internal error"; // ----->
}


OpenSSL::TSSLErrorCode OpenSSL::getSSLErrorCode(TSSL *ssl, int const &code) {
    return SSL_get_error(ssl, code);
}


void assertOK(int const &code, string const &message) {
    if (code <= 0)
        throw std::runtime_error(message + ": " + OpenSSL::instance().getErrorString()); // ----->
}


void assertOK(OpenSSL::TSSL *ssl, int const &code, string const &message) {
    if (ssl && OpenSSL::TSSLErrorCode(code) != OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_NONE)
        throw std::runtime_error(message + ": " + OpenSSL::instance().getSSLErrorString(ssl, code)); // ----->
}


template<typename TValue>
TValue assertOK(TValue value, string const &message) {
    return std::move(iridium::assertExists(value, message + ": " + OpenSSL::instance().getErrorString())); // ----->
}


OpenSSL::TContext *OpenSSL::createContext(bool const &is_server_method) {
    LOGT << "server method = " << is_server_method;
    SSL_METHOD const *method = nullptr;

    if (is_server_method)
        method = assertOK(TLS_server_method(), "openssl server method init tls error");
    else
        method = assertOK(TLS_client_method(), "openssl client method init tls error");

    return assertOK(SSL_CTX_new(method), "openssl init context error"); // ----->
}


void OpenSSL::configureContext(
    TContext  * const  context,
    std::string const &file_name_private_key,
    std::string const &file_name_certificate)
{
//    LOGT;
    // configure context
    SSL_CTX_set_ecdh_auto(context, 1);
    assertOK(SSL_CTX_use_certificate_file(context, file_name_certificate.c_str(), SSL_FILETYPE_PEM), "openssl use certificate error");
    assertOK(SSL_CTX_use_PrivateKey_file (context, file_name_private_key.c_str(), SSL_FILETYPE_PEM), "openssl use private key error");

//    LOGT << file_name_private_key << " " << file_name_certificate;
}


void OpenSSL::releaseContext(TContext *context) {
    LOGT;
    SSL_CTX_free(context);
}


OpenSSL::TSSL *OpenSSL::createSSL(TContext *context, int const &fd) {
    LOGT;
    auto ssl = assertOK(SSL_new(context), "openssl creating ssl error");
    assertOK(SSL_set_fd(ssl, fd), "openssl creating ssl set fd error");
    return ssl; // ----->
}


void OpenSSL::releaseSSL(TSSL *ssl) {
    LOGT;
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
}


void OpenSSL::acceptSSL(TSSL *ssl, bool const &is_blocking_mode) {
    LOGT << "is_blocking_mode = " << is_blocking_mode;

    if (!ssl)
        throw std::runtime_error("openssl accepting error: ssl is null");

    auto accept_result  = SSL_accept(ssl);
    if ( accept_result <= 0) {
        TSSLErrorCode code = SSL_get_error(ssl, accept_result);
        if (is_blocking_mode || !checkOneOf(code,
            TSSLErrorCode::SSL_ERROR_CODE_WANT_READ,
            TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE,
            TSSLErrorCode::SSL_ERROR_CODE_NONE))
        {
            assertOK(ssl, code, "openssl accepting error");
        } else {
            // todo:
        }
    }

    setAcceptState(ssl);
}


OpenSSL::TSSLErrorCode OpenSSL::connectSSL(TSSL *ssl) {
    auto code   = SSL_connect(ssl);
    auto error  = getSSLErrorCode(ssl, code);

    LOGT << error << " code " << code << " error: " << getErrorString();
    return error; // ----->
}


OpenSSL::TSSLErrorCode OpenSSL::setConnectState(TSSL *ssl) {
    SSL_set_connect_state(ssl);
    auto error = getErrorString();
    LOGT << error;
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    return TSSLErrorCode::SSL_ERROR_CODE_NONE; // ----->
}


OpenSSL::TSSLErrorCode OpenSSL::setAcceptState(TSSL *ssl) {
    SSL_set_accept_state(ssl);
    auto error = getErrorString();
    LOGT << error;
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    return TSSLErrorCode::SSL_ERROR_CODE_NONE; // ----->
}


OpenSSL::TSSLErrorCode OpenSSL::doHandshake(TSSL *ssl) {
    auto code = SSL_do_handshake(ssl);
    if  (code == 1)
        return OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_NONE; // ----->
    else {
        auto ssl_code =  getSSLErrorCode(ssl, code);
        if  (ssl_code == OpenSSL::TSSLErrorCode::SSL_ERROR_CODE_SSL)
            throw std::runtime_error("socket ssl handshake error: " +
                OpenSSL::instance().getSSLErrorString(ssl, code) + ", " +
                OpenSSL::instance().getErrorString()); // ----->
        return ssl_code; // ----->
    }
}


size_t OpenSSL::write(TSSL *ssl, io::Buffer::TSharedPtr const &packet) {
    assertOK(SSL_write(ssl, packet->data(), packet->size()), "openssl writing error");
//    LOGT << "ssl write: " << *packet;
    return packet->size(); // ----->
}


io::Buffer::TSharedPtr OpenSSL::read(TSSL *ssl, size_t const &size) {
//    static size_t const DEFAULT_SOCKET_BUFFER_SIZE = 16384;

//    char    buffer[DEFAULT_SOCKET_BUFFER_SIZE];
    char    buffer[size];

    auto result = SSL_read(ssl, buffer, size);

//    LOGT << "read result = " << result << "\n" <<
//            (result > 0 ? io::Buffer(buffer, buffer + (int)result) : string());

//    if (read_result == 0)
//        throw std::runtime_error("openssl reading error: socket was closed by client"); // ----->
    if (result == 0)
        return io::Buffer::create(); // ----->

    if (result <  0) {
        auto code = getSSLErrorCode(ssl, result);
//        LOGT << "ssl error " << getSSLErrorString(ssl, code) << " code " << code;
        if (code == TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
            code == TSSLErrorCode::SSL_ERROR_CODE_NONE)
            return {}; // ----->
        else
            throw std::runtime_error("openssl reading error: " + getSSLErrorString(ssl, code));
    }

    return io::Buffer::create(io::Buffer(buffer, buffer + (int)result)); // ----->
}


//CContext::CContext(
//    bool        const &is_blocking_mode,
//    std::string const &file_name_private_key,
//    std::string const &file_name_certificate)
//:
//    m_is_blocking_mode(is_blocking_mode)
//{
//    m_context = API::instance().createContext();
//    API::instance().configureContext(m_context, file_name_private_key, file_name_certificate);
//}


//CContext::~CContext() {
//    API::instance().releaseContext(m_context);
//}


//ISSL::TSharedPtr CContext::accept(int const &fd) {
//    LOGT << "fd " << fd;
//    CSSL::TSharedPtr ssl = CSSL::create(shared_from_this(), fd, m_is_blocking_mode);
//    ssl->accept();
//    return ssl;
//}


//CContext::CSSL::CSSL(CContext::TSharedPtr const &context, int const &fd, bool const &is_blocking_mode)
//:
//    m_is_blocking_mode  (is_blocking_mode),
//    m_ssl               (API::instance().createSSL(context->m_context, fd))
//{}


//CContext::CSSL::~CSSL() {
//    API::instance().releaseSSL(m_ssl);
//}


//size_t CContext::CSSL::write(io::Buffer::TSharedPtr const &packet) {
//    API::instance().write(m_ssl, packet);
//    return packet->size();
//}


//io::Buffer::TSharedPtr CContext::CSSL::read(size_t const &size) {
//    return API::instance().read(m_ssl, size); // ----->
//}


//int CContext::CSSL::getID() const {
//    return {};
//}


//void CContext::CSSL::initialize() {
//}


//void CContext::CSSL::finalize() {
//}


//void CContext::CSSL::accept() {
//    API::instance().acceptSSL(m_ssl, m_is_blocking_mode);
//}


//} // openssl
//} // implementation
} // encryption
} // iridium


#endif // BUILD_FLAG_OPENSSL
