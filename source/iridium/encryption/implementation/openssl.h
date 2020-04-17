// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8
#define HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8


#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_OPENSSL


#include "iridium/pattern/singleton.h"
#include "iridium/io/net/socket.h"
#include "iridium/smart_ptr.h"
#include "iridium/enum.h"

#include "iridium/encryption/keys.h"
#include "iridium/encryption/ssl.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include <memory>


namespace iridium {
namespace encryption {
namespace implementation {
namespace openssl {


class API: public pattern::Singleton<API> {
public:
    typedef SSL_CTX TContext;
    typedef SSL     TSSL;
    virtual ~API();

    DEFINE_ENUM(
        TErrorCode,
        ERROR_CODE_R_SYS_LIB    = ERR_R_SYS_LIB,
        ERROR_CODE_R_BN_LIB     = ERR_R_BN_LIB,
        ERROR_CODE_R_RSA_LIB    = ERR_R_RSA_LIB,
        ERROR_CODE_R_DH_LIB     = ERR_R_DH_LIB,
        ERROR_CODE_R_EVP_LIB    = ERR_R_EVP_LIB,
        ERROR_CODE_R_BUF_LIB    = ERR_R_BUF_LIB,
        ERROR_CODE_R_OBJ_LIB    = ERR_R_OBJ_LIB,
        ERROR_CODE_R_PEM_LIB    = ERR_R_PEM_LIB,
        ERROR_CODE_R_DSA_LIB    = ERR_R_DSA_LIB,
        ERROR_CODE_R_X509_LIB   = ERR_R_X509_LIB,
        ERROR_CODE_R_ASN1_LIB   = ERR_R_ASN1_LIB,
//        ERROR_CODE_R_CONF_LIB   = ERR_R_CONF_LIB,
//        ERROR_CODE_R_CRYPTO_LIB = ERR_R_CRYPTO_LIB,
        ERROR_CODE_R_EC_LIB     = ERR_R_EC_LIB,
//        ERROR_CODE_R_SSL_LIB    = ERR_R_SSL_LIB,
        ERROR_CODE_R_BIO_LIB    = ERR_R_BIO_LIB,
        ERROR_CODE_R_PKCS7_LIB  = ERR_R_PKCS7_LIB,
        ERROR_CODE_R_X509V3_LIB = ERR_R_X509V3_LIB,
//        ERROR_CODE_R_PKCS12_LIB = ERR_R_PKCS12_LIB,
//        ERROR_CODE_R_RAND_LIB   = ERR_R_RAND_LIB,
//        ERROR_CODE_R_DSO_LIB    = ERR_R_DSO_LIB,
        ERROR_CODE_R_ENGINE_LIB = ERR_R_ENGINE_LIB,
//        ERROR_CODE_R_OCSP_LIB   = ERR_R_OCSP_LIB,
        ERROR_CODE_R_UI_LIB     = ERR_R_UI_LIB,
//        ERROR_CODE_R_COMP_LIB   = ERR_R_COMP_LIB,
        ERROR_CODE_R_ECDSA_LIB  = ERR_R_ECDSA_LIB,
//        ERROR_CODE_R_ECDH_LIB   = ERR_R_ECDH_LIB,
//        ERROR_CODE_R_STORE_LIB  = ERR_R_STORE_LIB,
//        ERROR_CODE_R_TS_LIB     = ERR_R_TS_LIB,

        ERROR_CODE_R_NESTED_ASN1_ERROR          = ERR_R_NESTED_ASN1_ERROR,
//        ERROR_CODE_R_BAD_ASN1_OBJECT_HEADER     = ERR_R_BAD_ASN1_OBJECT_HEADER,
//        ERROR_CODE_R_BAD_GET_ASN1_OBJECT_CALL   = ERR_R_BAD_GET_ASN1_OBJECT_CALL,
//        ERROR_CODE_R_EXPECTING_AN_ASN1_SEQUENCE = ERR_R_EXPECTING_AN_ASN1_SEQUENCE,
//        ERROR_CODE_R_ASN1_LENGTH_MISMATCH       = ERR_R_ASN1_LENGTH_MISMATCH,
        ERROR_CODE_R_MISSING_ASN1_EOS           = ERR_R_MISSING_ASN1_EOS,

        ERROR_CODE_R_FATAL                          = ERR_R_FATAL,
        ERROR_CODE_R_MALLOC_FAILURE                 = ERR_R_MALLOC_FAILURE,
        ERROR_CODE_R_SHOULD_NOT_HAVE_BEEN_CALLED    = ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED,
        ERROR_CODE_R_PASSED_NULL_PARAMETER          = ERR_R_PASSED_NULL_PARAMETER,
        ERROR_CODE_R_INTERNAL_ERROR                 = ERR_R_INTERNAL_ERROR,
        ERROR_CODE_R_DISABLED                       = ERR_R_DISABLED
    )

    DEFINE_ENUM(
        TSSLErrorCode,
        SSL_ERROR_CODE_NONE             = SSL_ERROR_NONE,
        SSL_ERROR_CODE_SSL              = SSL_ERROR_SSL,
        SSL_ERROR_CODE_WANT_READ        = SSL_ERROR_WANT_READ,
        SSL_ERROR_CODE_WANT_WRITE       = SSL_ERROR_WANT_WRITE,
        SSL_ERROR_CODE_WANT_X509_LOOKUP = SSL_ERROR_WANT_X509_LOOKUP,
        SSL_ERROR_CODE_SYSCALL          = SSL_ERROR_SYSCALL,
        SSL_ERROR_CODE_ZERO_RETURN      = SSL_ERROR_ZERO_RETURN,
        SSL_ERROR_CODE_WANT_CONNECT     = SSL_ERROR_WANT_CONNECT,
        SSL_ERROR_CODE_WANT_ACCEPT      = SSL_ERROR_WANT_ACCEPT
    )

    TContext *createContext(
        std::string const &file_name_private_key,
        std::string const &file_name_certificate);

    void releaseContext(TContext *context);

    TSSL *createSSL(TContext *context, int const &file_descriptor);

    void releaseSSL(TSSL *ssl);

    void acceptSSL(TSSL *ssl, bool const &is_blocking_mode);

    TSSLErrorCode connectSSL(TSSL *ssl);

    TSSLErrorCode doHandshake(TSSL *ssl);

    size_t write(TSSL *ssl, io::Buffer::TSharedPtr const &packet);

    io::Buffer::TSharedPtr read(TSSL *ssl, size_t const &size);

    std::string getErrorString();
    std::string getErrorString(int const &code);

    std::string getSSLErrorString(TSSL *ssl, int const &code);
    TSSLErrorCode getSSLErrorCode(TSSL *ssl, int const &code);

private:
    friend class pattern::Singleton<API>;
    API();
};


class CContext:
    public IContext,
    public std::enable_shared_from_this<CContext>
{
public:
    CContext(
        bool        const &is_blocking_mode         = false,
        std::string const &file_name_private_key    = DEFAULT_FILE_NAME_PRIVATE_KEY,
        std::string const &file_name_certificate    = DEFAULT_FILE_NAME_CERTIFICATE);
    virtual ~CContext();
    DEFINE_CREATE(CContext)

    class CSSL: public ISSL {
    public:
        CSSL(CContext::TSharedPtr const &context, int const &fd, bool const &is_blocking_mode);
        virtual ~CSSL();
        DEFINE_CREATE(CSSL)

        size_t write(io::Buffer::TSharedPtr const &packet) override;
        io::Buffer::TSharedPtr read(size_t const &size) override;
        int getID() const override;
        void initialize() override;
        void finalize() override;
    private:
        friend class CContext;
        void accept();

        bool        m_is_blocking_mode;
        API::TSSL  *m_ssl;
    };

    ISSL::TSharedPtr accept(int const &fd) override;

private:
    bool            m_is_blocking_mode;
    API::TContext  *m_context;
};


} // openssl
} // implementation
} // encryption
} // iridium


#endif // BUILD_FLAG_OPENSSL


#endif // HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8
