#ifndef HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E
#define HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io//stream.h"
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


class CStreamPortAcceptor: public IStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortAcceptor)
    CStreamPortAcceptor() = default;

    void initialize() override {};
    void finalize()   override {};
};


} // unix
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E
