// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ACCEPTOR_E59CB9D6_A4C2_4CDC_870E_E146CFFA0506
#define HEADER_ACCEPTOR_E59CB9D6_A4C2_4CDC_870E_E146CFFA0506


#include "protocol.h"


namespace iridium {
namespace io {


class IAcceptor {
public:
    DEFINE_INTERFACE(IAcceptor)
    virtual IProtocol::TSharedPtr accept(URI::TSharedPtr const &uri) = 0;
};


} // io
} // iridium


#endif // HEADER_ACCEPTOR_E59CB9D6_A4C2_4CDC_870E_E146CFFA0506
