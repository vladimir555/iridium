#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_19198010_6870_4A6B_A997_4F4B2E87311D
#define HEADER_SHARED_MEMORY_STREAM_PORT_19198010_6870_4A6B_A997_4F4B2E87311D


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
        throw std::runtime_error("not implemented for emscripten");
    }
    virtual ~CSharedMemoryPort();
};


} // platform
} // implementation
} // io
} // iridium


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_19198010_6870_4A6B_A997_4F4B2E87311D
