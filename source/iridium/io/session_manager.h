// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72
#define HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72


#include "iridium/pattern/initializable.h"

#include "protocol.h"


namespace iridium {
namespace io {


class ISessionManager: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(ISessionManager)

    virtual void manage(
        IStreamPort::TSharedPtr const &stream,
        IProtocol::TSharedPtr   const &protocol) = 0;
};


} // io
} // iridium


#endif // HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72
