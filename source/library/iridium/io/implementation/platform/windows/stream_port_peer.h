#ifndef HEADER_STREAM_PORT_PEER_WINDOWS_5E6F7A8B
#define HEADER_STREAM_PORT_PEER_WINDOWS_5E6F7A8B


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port.h"


namespace iridium::io::implementation::platform {


class CStreamPortPeer: public CStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortPeer)
    explicit CStreamPortPeer(URI const &uri);
    CStreamPortPeer(URI const &uri, int const &fd);

    void initialize() override;
    void finalize() override;
};


} // namespace


#endif // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_WINDOWS_5E6F7A8B
