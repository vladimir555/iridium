/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "openssl.h"


#ifdef BUILD_FLAG_OPENSSL


#include "iridium/logging/logger.h"
#include "iridium/assert.h"

#include "iridium/threading/thread.h"
//#include <signal.h>


using std::string;
using iridium::convertion::convert;


IMPLEMENT_ENUM(iridium::encryption::implementation::openssl::API::TErrorCode)
IMPLEMENT_ENUM(iridium::encryption::implementation::openssl::API::TSSLErrorCode)


//// todo: signals handlers singleton
//void handleSignal(int signal) {
//    LOGT << "broken pipe signal " << signal;
//}


namespace iridium {
namespace encryption {
namespace implementation {
namespace openssl {


API::API() {
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


API::~API() {
    ERR_free_strings();
    EVP_cleanup();
}


string API::getErrorString() {
    BIO    *bio     = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char   *buffer  = nullptr;
    size_t  length  = BIO_get_mem_data(bio, &buffer);
//    char   *result  = static_cast<char *>(calloc(1, 1 + length));
//    if (result)
//        memcpy(result, buffer, length);
//    BIO_free(bio);
//    if (result)
//        return result; // ----->
//    else
//        return string(); // ----->
    return string(buffer, length); // ----->
}


string API::getSSLErrorString(TSSL *ssl, int const &code_) {
    auto code = SSL_get_error(ssl, code_);
    return std::string(ERR_error_string(code, nullptr)) +
        ", error code: " + convert<std::string>(static_cast<TSSLErrorCode>(code)); // ----->
}


API::TSSLErrorCode API::getSSLErrorCode(TSSL *ssl, int const &code) {
    return SSL_get_error(ssl, code);
}


void assertOK(int const &code, string const &message) {
    if (code <= 0)
        throw std::runtime_error(message + ": " + API::instance().getErrorString()); // ----->
}


void assertOK(API::TSSL *ssl, int const &code, string const &message) {
    if (ssl && API::TSSLErrorCode(code) != API::TSSLErrorCode::SSL_ERROR_CODE_NONE)
        throw std::runtime_error(message + ": " + API::instance().getSSLErrorString(ssl, code)); // ----->
}


template<typename TValue>
TValue assertOK(TValue value, string const &message) {
    return std::move(iridium::assertExists(value, message + ": " + API::instance().getErrorString())); // ----->
}


API::TContext *API::createContext(bool const &is_client_method) {
    SSL_METHOD const *method = nullptr;

    if (is_client_method)
        method = assertOK(TLS_client_method(), "openssl client method init tls error");
    else
        method = assertOK(TLS_server_method(), "openssl server method init tls error");

    return assertOK(SSL_CTX_new(method), "openssl init context error"); // ----->
}


void API::configureContext(
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


void API::releaseContext(TContext *context) {
    LOGT;
    SSL_CTX_free(context);
}


API::TSSL *API::createSSL(TContext *context, int const &fd) {
    LOGT;
    auto ssl = assertOK(SSL_new(context), "openssl creating ssl error");
    assertOK(SSL_set_fd(ssl, fd), "openssl creating ssl set fd error");
    return ssl; // ----->
}


void API::releaseSSL(TSSL *ssl) {
    LOGT;
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
}


void API::acceptSSL(TSSL *ssl, bool const &is_blocking_mode) {
    LOGT << "is_blocking_mode = " << is_blocking_mode;
    auto accept_result = SSL_accept(ssl);
    if (accept_result <= 0) {
        TSSLErrorCode code = SSL_get_error(ssl, accept_result);
        if (is_blocking_mode || (
            code != TSSLErrorCode::SSL_ERROR_CODE_WANT_READ     &&
            code != TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE    &&
            code != TSSLErrorCode::SSL_ERROR_CODE_NONE)
        )
            assertOK(ssl, code, "openssl accepting error");
    }
    SSL_set_accept_state(ssl);
}


API::TSSLErrorCode API::connectSSL(TSSL *ssl) {
    auto code   = SSL_connect(ssl);
    auto error  = getSSLErrorCode(ssl, code);
//    LOGT << error;
    return error;
}


API::TSSLErrorCode API::doHandshake(TSSL *ssl) {
    auto code = SSL_do_handshake(ssl);
    if  (code == 1)
        return API::TSSLErrorCode::SSL_ERROR_CODE_NONE;
    else
        return getSSLErrorCode(ssl, code);
}


size_t API::write(TSSL *ssl, io::Buffer::TSharedPtr const &packet) {
//    LOGT << SSL_write(ssl, packet->data(), packet->size());
    assertOK(SSL_write(ssl, packet->data(), packet->size()), "openssl writing error");
    //todo:
    LOGT << "ssl write: " << *packet;
    return packet->size();
}


io::Buffer::TSharedPtr API::read(TSSL *ssl, size_t const &size) {
    static size_t const DEFAULT_SOCKET_BUFFER_SIZE = 16384;

    char    buffer[DEFAULT_SOCKET_BUFFER_SIZE];
    int     read_result = -1;
    size_t  received_bytes = 0;

    read_result = SSL_read_ex(ssl, buffer, size, &received_bytes);
    LOGT << "ssl read received_size: " << received_bytes;

//    if (read_result == 0)
//        throw std::runtime_error("openssl reading error: socket was closed by client"); // ----->
    if (read_result == 0)
        return io::Buffer::create(); // ----->

    if (read_result <  0) {
        TSSLErrorCode code = SSL_get_error(ssl, read_result);
        LOGT << "ssl error " << getSSLErrorString(ssl, code);
        if (code == TSSLErrorCode::SSL_ERROR_CODE_WANT_READ ||
            code == TSSLErrorCode::SSL_ERROR_CODE_NONE)
            return {}; // ----->
        else
            throw std::runtime_error("openssl reading error: " + getSSLErrorString(ssl, code));
    }

    return io::Buffer::create(buffer, buffer + read_result); // ----->
}


CContext::CContext(
    bool        const &is_blocking_mode,
    std::string const &file_name_private_key,
    std::string const &file_name_certificate)
:
    m_is_blocking_mode(is_blocking_mode)
{
    m_context = API::instance().createContext();
    API::instance().configureContext(m_context, file_name_private_key, file_name_certificate);
}


CContext::~CContext() {
    API::instance().releaseContext(m_context);
}


ISSL::TSharedPtr CContext::accept(int const &fd) {
    LOGT << "fd " << fd;
    CSSL::TSharedPtr ssl = CSSL::create(shared_from_this(), fd, m_is_blocking_mode);
    ssl->accept();
    return ssl;
}


CContext::CSSL::CSSL(CContext::TSharedPtr const &context, int const &fd, bool const &is_blocking_mode)
:
    m_is_blocking_mode  (is_blocking_mode),
    m_ssl               (API::instance().createSSL(context->m_context, fd))
{}


CContext::CSSL::~CSSL() {
    API::instance().releaseSSL(m_ssl);
}


size_t CContext::CSSL::write(io::Buffer::TSharedPtr const &packet) {
    API::instance().write(m_ssl, packet);
    return packet->size();
}


io::Buffer::TSharedPtr CContext::CSSL::read(size_t const &size) {
    return API::instance().read(m_ssl, size); // ----->
}


int CContext::CSSL::getID() const {
    return {};
}


void CContext::CSSL::initialize() {
}


void CContext::CSSL::finalize() {
}


void CContext::CSSL::accept() {
    API::instance().acceptSSL(m_ssl, m_is_blocking_mode);
}


} // openssl
} // implementation
} // encryption
} // iridium


#endif // BUILD_FLAG_OPENSSL
