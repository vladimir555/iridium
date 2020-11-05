// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B
#define HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B


#include "../unix/socket.h"


#ifdef LINUX_PLATFORM
#ifdef BUILD_FLAG_OPENSSL


#include "iridium/encryption/openssl.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


class CSocket: public unix::CSocket {
public:
    DEFINE_IMPLEMENTATION(CSocket)
    CSocket(URL const &url, bool const &is_server_mode);

    void initialize() override;
    void finalize() override;

    ISocket::TSharedPtr accept() override;

    size_t write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr read(size_t const &size) override;

private:
    bool initializeAsync();

    encryption::OpenSSL::TContext  *m_context;
    encryption::OpenSSL::TSSL      *m_ssl;
    int m_ssl_init_step;
};


} // platform
} // implementation
} // net
} // io
} // iridium


#else
namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocket CSocket;


} // platform
} // implementation
} // net
} // io
} // iridium
#endif // BUILD_FLAG_OPENSSL
#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B
