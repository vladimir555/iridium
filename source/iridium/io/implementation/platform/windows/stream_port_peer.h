#ifndef HEADER_STREAM_PORT_PEER_WINDOWS_5E6F7A8B
#define HEADER_STREAM_PORT_PEER_WINDOWS_5E6F7A8B


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
