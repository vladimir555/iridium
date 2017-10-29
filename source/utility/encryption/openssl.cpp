#include "openssl.h"


#ifdef BUILD_FLAG_OPENSSL


#include "utility/logging/logger.h"
#include "utility/assert.h"


using std::string;
using utility::convertion::convert;


IMPLEMENT_ENUM(utility::encryption::openssl::API::TErrorCode)
IMPLEMENT_ENUM(utility::encryption::openssl::API::TSSLErrorCode)


namespace utility {
namespace encryption {
namespace openssl {


API::API() {
    LOGT << "initialize openssl";
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
}


API::~API() {
    LOGT << "finalize openssl";
    ERR_free_strings();
    EVP_cleanup();
}


string API::getErrorString() {
    BIO    *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char   *buffer  = nullptr;
    size_t  length  = BIO_get_mem_data(bio, &buffer);
    char   *result  = static_cast<char *>(calloc(1, 1 + length));
    if (result)
        memcpy(result, buffer, length);
    BIO_free(bio);
    return result; // ----->
}


string API::getSSLErrorString(TSSL *ssl, int const &code_) {
    auto code = SSL_get_error(ssl, code_);
    return std::string(ERR_error_string(code, nullptr)) +
        ", error code: " + convert<std::string>(static_cast<TSSLErrorCode>(code)); // ----->
}


void assertOK(int const &code, string const &message) {
//    LOGT << code;
    if (code <= 0)
        throw std::runtime_error(message + ": " + API::instance().getErrorString()); // ----->
}


void assertOK(API::TSSL *ssl, int const &code, string const &message) {
    if (ssl && code != API::TSSLErrorCode::SSL_ERROR_CODE_NONE)
        throw std::runtime_error(message + ": " + API::instance().getSSLErrorString(ssl, code)); // ----->
}


template<typename T>
T assertOK(T t, string const &message) {
    return std::move(utility::assertExists(t, message + ": " + API::instance().getErrorString())); // ----->
}


API::TContext *API::createContext(std::string const &file_name_private_key, std::string const &file_name_certificate) {
    // create context
    auto const *method  = assertOK(SSLv23_server_method(), "openssl init ssl v23 error");
    auto       *context = assertOK(SSL_CTX_new(method),    "openssl init context error");

    // configure context
    SSL_CTX_set_ecdh_auto(context, 1);
    assertOK(SSL_CTX_use_certificate_file(context, file_name_certificate.c_str(), SSL_FILETYPE_PEM), "openssl use certificate error");
    assertOK(SSL_CTX_use_PrivateKey_file (context, file_name_private_key.c_str(), SSL_FILETYPE_PEM), "openssl use private key error");

    LOGT << file_name_private_key << " " << file_name_certificate;
    return context; // ----->
}


void API::releaseContext(TContext *context) {
    LOGT << "release context";
    SSL_CTX_free(context);
}


API::TSSL *API::createSSL(TContext *context, int const &fd) {
//    LOGT << "create ssl";
    auto ssl = assertOK(SSL_new(context), "openssl creating ssl error");
    assertOK(SSL_set_fd(ssl, fd), "openssl creating ssl set fd error");
    return ssl; // ----->
}


void API::releaseSSL(TSSL *descriptor) {
//    LOGT << "release ssl";
    SSL_shutdown(descriptor);
    SSL_free(descriptor);
}


void API::acceptSSL(TSSL *ssl, bool const &is_blocking_mode) {
//    LOGT << "accept ssl";

    auto accept_result = SSL_accept(ssl);
    if (accept_result <= 0) {
        TSSLErrorCode code = SSL_get_error(ssl, accept_result);
//        LOGT << "accept return code " << code;
        if (is_blocking_mode || (
            code != TSSLErrorCode::SSL_ERROR_CODE_WANT_READ &&
            code != TSSLErrorCode::SSL_ERROR_CODE_WANT_WRITE)
        )
            assertOK(ssl, code, "openssl accepting error");
    }
}


void API::write(TSSL *ssl, networking::ISocket::TPacket const &packet) {
//    LOGT << "write ssl";
    assertOK(SSL_write(ssl, packet.data(), packet.size()), "openssl writing error");
}


networking::ISocket::TPacket API::read(TSSL *ssl, size_t const &size) {
//    LOGT << "read ssl size " << size;

    static size_t const DEFAULT_SOCKET_BUFFER_SIZE = 512;

    char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
    int received_size = -1;
//    while (received_size < 0) {
    received_size = SSL_read(ssl, buffer, size);
//    LOGT << "read ssl received " << received_size;
//    }

    if (received_size <= 0)
        return networking::ISocket::TPacket();

    return networking::ISocket::TPacket(buffer, buffer + received_size); // ----->
}


Context::Context(bool const &is_blocking_mode, std::string const &file_name_private_key, std::string const &file_name_certificate)
:
    m_is_blocking_mode(is_blocking_mode)
{
    m_context = API::instance().createContext(file_name_private_key, file_name_certificate);
}


Context::~Context() {
    API::instance().releaseContext(m_context);
}


Context::SSL::TSharedPtr Context::accept(int const &fd) {
    SSL::TSharedPtr ssl = SSL::create(shared_from_this(), fd, m_is_blocking_mode);
//    LOGT << "accept " << fd;
    ssl->accept();
    return ssl;
}


Context::SSL::SSL(Context::TSharedPtr const &context, int const &fd, bool const &is_non_blocking)
:
    m_is_blocking_mode  (is_non_blocking),
    m_ssl               (API::instance().createSSL(context->m_context, fd))
{}


Context::SSL::~SSL() {
    API::instance().releaseSSL(m_ssl);
}


void Context::SSL::write(networking::ISocket::TPacket const &packet) {
    API::instance().write(m_ssl, packet);
}


networking::ISocket::TPacket Context::SSL::read(size_t const &size) {
    return API::instance().read(m_ssl, size); // ----->
}


void Context::SSL::accept() {
    API::instance().acceptSSL(m_ssl, m_is_blocking_mode);
}


} // openssl
} // encryption
} // utility


#endif // BUILD_FLAG_OPENSSL
