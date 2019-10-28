#ifndef HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
#define HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC


#include "iridium/net/socket.h"


namespace iridium {
namespace encryption {


class ISSL {
public:
    virtual ~ISSL() = default;
    DEFINE_SMART_PTR(ISSL)
    virtual void write(net::ISocket::TPacket const &packet) = 0;
    virtual net::ISocket::TPacket read(size_t const &size) = 0;
};


class IContext {
public:
    virtual ~IContext() = default;
    DEFINE_SMART_PTR(IContext)
    virtual ISSL::TSharedPtr accept(int const &fd) = 0;
};


} // encryption
} // iridium


#endif // HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
