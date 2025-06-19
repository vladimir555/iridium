#ifndef HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE
#define HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/implementation/stream_port.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <string>
#include <cstring>
#include <atomic>
#include <list>
#include <set>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


class CStreamPortPeer: public io::implementation::CStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortPeer)
    CStreamPortPeer(URI const &uri, int const &fd);

    void initialize() override;
    void finalize() override;
};


} // unix_
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_PEER_AC935D90_653E_4C59_8ED4_07CBA37022CE
