#ifndef HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE
#define HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/implementation/stream_port.h"
#include "iridium/convertion/convert.h"
#include "iridium/dp/non_copyable.h"

#include <string>
#include <cstring>
#include <atomic>
#include <list>
#include <set>


namespace iridium::io::implementation::platform::unix_ {


class CStreamPortPeer: public io::implementation::CStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortPeer)
    CStreamPortPeer(URI const &uri, int const &fd);

    void initialize() override;
    void finalize() override;
};


} // iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE
