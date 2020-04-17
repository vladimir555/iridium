// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
#define HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC


#include "iridium/io/net/socket.h"


namespace iridium {
namespace encryption {


class ISSL: public io::IStreamPort {
public:
    DEFINE_INTERFACE(ISSL)
};


class IContext {
public:
    DEFINE_INTERFACE(IContext)
    virtual ISSL::TSharedPtr accept(int const &fd) = 0;
};


} // encryption
} // iridium


#endif // HEADER_SSL_84E083DB_766C_4092_A0B2_0781653E5DBC
