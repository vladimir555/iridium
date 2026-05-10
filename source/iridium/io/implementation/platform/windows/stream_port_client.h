#ifndef HEADER_STREAM_PORT_CLIENT_WINDOWS_1A2B3C4D
#define HEADER_STREAM_PORT_CLIENT_WINDOWS_1A2B3C4D


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port.h"
#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/pattern/non_copyable.h"


namespace iridium::io::implementation::platform {


class CStreamPortClient: public CStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortClient)
    explicit CStreamPortClient(URI const &uri);

    void initialize() override;
    void finalize() override;
};


} // namespace


#endif // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_CLIENT_WINDOWS_1A2B3C4D
