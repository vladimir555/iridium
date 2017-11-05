#ifndef HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
#define HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC


#include "utility/networking/socket.h"


namespace utility {
namespace encryption {


class ISSL {
public:
    virtual ~ISSL() = default;
    DEFINE_SMART_PTR(ISSL)
    virtual void write(networking::ISocket::TPacket const &packet) = 0;
    virtual networking::ISocket::TPacket read(size_t const &size) = 0;
};


class IContext {
public:
    virtual ~IContext() = default;
    DEFINE_SMART_PTR(IContext)
    virtual ISSL::TSharedPtr accept(int const &fd) = 0;
};


} // encryption
} // utility


#endif // HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
