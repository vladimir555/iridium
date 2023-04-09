// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
#define HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC


#include "iridium/io/client.h"
#include "iridium/io/url.h"
#include "iridium/io/session.h"


namespace iridium {
namespace io {
namespace implementation {


class CClient: public IClient {
public:
    DEFINE_IMPLEMENTATION(CClient)
    CClient(URL const &url, IProtocol::TSharedPtr const &protocol);
    void initialize()   override;
    void finalize()     override;

private:
    IStreamPort::TSharedPtr     m_stream;
    IProtocol::TSharedPtr       m_protocol;
    ISessionManager::TSharedPtr m_session_manager;
};


} // implementation
} // io
} // iridium


#endif // HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
