#ifndef HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C
#define HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C


#include "utility/build_flags.h"


#ifdef BUILD_FLAG_OPENSSL
#include "openssl.h"
#else
#include "utility/smart_ptr.h"
#include "utility/encryption/keys.h"
#include "utility/encryption/ssl.h"
#include <string>
#endif // BUILD_FLAG_OPENSSL


namespace utility {
namespace encryption {
namespace implementation {


#ifdef BUILD_FLAG_OPENSSL
typedef openssl::CContext CContext;
#else
// fake ssl context
class CContext: public IContext {
public:
    CContext(
        bool        const &is_blocking_mode         = false,
        std::string const &file_name_private_key    = DEFAULT_FILE_NAME_PRIVATE_KEY,
        std::string const &file_name_certificate    = DEFAULT_FILE_NAME_CERTIFICATE)
    {
        throw std::runtime_error("SSL not implemented"); // ----->
    }
    virtual ~CContext() = default;
    DEFINE_SMART_PTR(CContext)
    DEFINE_CREATE(CContext)

    class CSSL: public ISSL {
    public:
        CSSL(CContext::TSharedPtr const &context, int const &fd, bool const &is_blocking_mode) {
            throw std::runtime_error("SSL not implemented"); // ----->
        }
        virtual ~CSSL() = default;
        DEFINE_SMART_PTR(CSSL)
        DEFINE_CREATE(CSSL)

        void write(networking::ISocket::TPacket const &packet) override {}
        networking::ISocket::TPacket read(size_t const &size) override {
            throw std::runtime_error("SSL not implemented"); // ----->
            return networking::ISocket::TPacket(); // ----->
        }
    };

    ISSL::TSharedPtr accept(int const &fd) override {
        throw std::runtime_error("SSL not implemented"); // ----->
        return nullptr; // ----->
    }
};
#endif // BUILD_FLAG_OPENSSL


} // implementation
} // encryption
} // utility


#endif // HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C
