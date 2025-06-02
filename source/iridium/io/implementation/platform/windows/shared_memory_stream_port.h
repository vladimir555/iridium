#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_D087B8F7_D949_4FFA_AD4E_C346AC46CB1D
#define HEADER_SHARED_MEMORY_STREAM_PORT_D087B8F7_D949_4FFA_AD4E_C346AC46CB1D


#include "iridium/platform.h"


#ifdef EMSCRIPTEN_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/pattern/non_copyable.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


class CSharedMemoryPort: virtual public IStreamPort, public pattern::NonCopyable {
protected:
    CSharedMemoryPort(URI const &) {
        throw std::runtime_error("not implemented for windows");
    }
    virtual ~CSharedMemoryPort();
};


} // platform
} // implementation
} // io
} // iridium


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_D087B8F7_D949_4FFA_AD4E_C346AC46CB1D
