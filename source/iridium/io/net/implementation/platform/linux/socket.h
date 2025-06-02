// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B
#define HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B


/// \~english @file socket.h
/// \~english @brief Defines socket implementations for the Linux platform.
/// \~english The behavior of this file is conditional based on the `BUILD_FLAG_OPENSSL` macro.
/// \~english If `BUILD_FLAG_OPENSSL` is defined, it's intended to provide OpenSSL-specific socket classes
/// \~english (though this functionality is largely commented out in the current version).
/// \~english Otherwise, it defaults to using typedefs to the common Unix socket implementations
/// \~english (`unix::CSocketPeer`, `unix::CSocketClient`, `unix::CSocketAcceptor`).
/// \~russian @file socket.h
/// \~russian @brief Определяет реализации сокетов для платформы Linux.
/// \~russian Поведение этого файла зависит от макроса `BUILD_FLAG_OPENSSL`.
/// \~russian Если `BUILD_FLAG_OPENSSL` определен, предполагается, что он предоставляет OpenSSL-специфичные классы сокетов
/// \~russian (хотя эта функциональность в значительной степени закомментирована в текущей версии).
/// \~russian В противном случае, он по умолчанию использует typedef для общих реализаций сокетов Unix
/// \~russian (`unix::CSocketPeer`, `unix::CSocketClient`, `unix::CSocketAcceptor`).

//#include "../unix/socket_acceptor.h" // These seem to be intended for the non-OpenSSL case, now handled by typedefs below.
//#include "../unix/socket_client.h"
//#include "../unix/socket_peer.h"


#ifdef LINUX_PLATFORM
#ifdef BUILD_FLAG_OPENSSL


#include "iridium/encryption/openssl.h"


//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {
//namespace platform {


////class CSocket: public unix::CSocket {
////public:
////    DEFINE_IMPLEMENTATION(CSocket)
////    CSocket(URI const &uri, TMode const &mode);
////    // peer
////    CSocket(URI const &uri, int const &fd, encryption::OpenSSL::TSSL *ssl);

////    void initialize() override;
////    void finalize() override;

////    ISocket::TSharedPtr accept() override;

////    size_t write(Buffer::TSharedPtr const &buffer) override;
////    Buffer::TSharedPtr read(size_t const &size) override;

////private:
////    bool    initializeSSLAsync();

////    encryption::OpenSSL::TContext  *m_context;
////    encryption::OpenSSL::TSSL      *m_ssl;
////    int     m_ssl_init_step;
////};


//} // platform
//} // implementation
//} // net
//} // io
//} // iridium


//#else
namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


typedef unix::CSocketPeer       CSocketPeer;
typedef unix::CSocketClient     CSocketClient;
typedef unix::CSocketAcceptor   CSocketAcceptor;


} // platform
} // implementation
} // net
} // io
} // iridium
#endif // BUILD_FLAG_OPENSSL
#endif // LINUX_PLATFORM


#endif // HEADER_SOCKET_952003F7_E74B_4AAA_AE03_D02905A2C89B
