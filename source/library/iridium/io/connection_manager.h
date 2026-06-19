// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTION_MANAGER_90AB8F44_3F72_46A3_B426_8492C7EFF5A6
#define HEADER_CONNECTION_MANAGER_90AB8F44_3F72_46A3_B426_8492C7EFF5A6


#include "acceptor.h"
#include "iridium/dp/initializable.h"
#include "iridium/enum.h"


namespace iridium::io {


class IConnectionManager: public dp::IInitializable {
public:
    DEFINE_INTERFACE(IConnectionManager)

    DEFINE_ENUM(
        THandleType,
        READER,
        WRITER,
        PID);

    // client
    virtual void manage(
        URI::TSharedPtr
            const &uri,
        IProtocol::TSharedPtr
            const &protocol) = 0;
    // server
    virtual void manage(
        URI::TSharedPtr
            const &uri,
        IAcceptor::TSharedPtr
            const &acceptor) = 0;
};


} // iridium::io


DEFINE_ENUM_CONVERT(iridium::io::IConnectionManager::THandleType)


#endif // HEADER_CONNECTION_MANAGER_90AB8F44_3F72_46A3_B426_8492C7EFF5A6
