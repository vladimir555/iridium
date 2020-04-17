// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C
#define HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C


#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_OPENSSL
#include "openssl.h"
#else
#include "iridium/smart_ptr.h"
#include "iridium/encryption/keys.h"
#include "iridium/encryption/ssl.h"
#include <string>
#endif // BUILD_FLAG_OPENSSL


namespace iridium {
namespace encryption {
namespace implementation {


class CContextDefault: public IContext {
public:
    DEFINE_IMPLEMENTATION(CContextDefault)
    CContextDefault(
        bool        const &is_blocking_mode         = false,
        std::string const &file_name_private_key    = DEFAULT_FILE_NAME_PRIVATE_KEY,
        std::string const &file_name_certificate    = DEFAULT_FILE_NAME_CERTIFICATE);

    class CSSL: public ISSL {
    public:
        DEFINE_IMPLEMENTATION(CSSL)
        CSSL(IContext::TSharedPtr const &context, int const &fd, bool const &is_blocking_mode);

        size_t write(io::Buffer::TSharedPtr const &packet) override;
        io::Buffer::TSharedPtr read(size_t const &size) override;

        int getID() const override;
        void initialize() override;
        void finalize() override;
    };

    ISSL::TSharedPtr accept(int const &fd) override;
};


#ifdef BUILD_FLAG_OPENSSL
typedef openssl::CContext CContext;
#else
typedef CContextDefault CContext;
#endif // BUILD_FLAG_OPENSSL


} // implementation
} // encryption
} // iridium


#endif // HEADER_SSL_994B9C4C_4FE3_4E23_BBA5_5FADE1EE091C
