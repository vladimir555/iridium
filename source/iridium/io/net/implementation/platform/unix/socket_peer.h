// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

//#ifndef HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D
//#define HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D


//#include "iridium/platform.h"


//#if defined(LINUX_PLATFORM) || defined(FREEBSD_LIKE_PLATFORM)


//#include "iridium/io/net/socket.h"
//#include "iridium/encryption/openssl.h"

//#include "socket.h"
//#include "socket_acceptor.h"


//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {
//namespace platform {
//namespace unix_ {


//class CSocketPeer: virtual public CSocketBase, public ISocketStream {
//public:
//    DEFINE_IMPLEMENTATION(CSocketPeer)

//    void initialize() override;
//    void finalize() override;

//    URI getURI() const override;
//    int getID() const override;
//    Buffer::TSharedPtr read(size_t const &size) override;
//    size_t write(Buffer::TSharedPtr const &buffer) override;

////protected:
////    friend class CSocketAcceptor;

//    CSocketPeer(int const &fd);
////    CSocketPeer(int const &fd, encryption::OpenSSL::TSSL *ssl, IListenerStreams::TSharedPtr const &listener);

//private:
//    URI::TProtocol              m_protocol;
////    encryption::OpenSSL::TSSL  *m_ssl;
//};


//} // unix
//} // platform
//} // implementation
//} // net
//} // io
//} // iridium


//#endif // LINUX_PLATFORM || FREEBSD_PLATFORM


//#endif // HEADER_SOCKET_PEER_A0DDF81D_C429_41EB_BA32_0F1C38A0390D
