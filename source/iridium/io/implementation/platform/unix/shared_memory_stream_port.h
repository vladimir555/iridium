#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F
#define HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <string>
#include <cstring>
#include <atomic>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


class CSharedMemoryStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
public:
    DEFINE_CREATE(CSharedMemoryStreamPort)
    CSharedMemoryStreamPort(URI const &uri);
    virtual ~CSharedMemoryStreamPort();

    virtual void            initialize() override;
    virtual void            finalize() override;
    size_t                  write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr      read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    std::list<uintptr_t>    getHandles() const override;
    URI::TSharedPtr         getURI() const override;

protected:
    void setBlockingMode(bool const &is_blocking);

    template<typename T>
    T assertOK(T result, std::string const &message);

    static int initSignal();

    std::atomic<int>    m_shm_fd;
    void*               m_shm_ptr;
    void*               m_data_ptr;
    size_t              m_shm_size;
    URI::TSharedPtr     m_uri;
    bool                m_is_opened;
    bool                m_is_blocking_mode;

private:
    std::atomic<int>*   m_refcount;
    std::string         m_shm_name;
};


template<typename T>
T CSharedMemoryStreamPort::assertOK(T result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_uri)
            throw std::runtime_error(message + ": uri " + convert<string>(*m_uri) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno));
        else
            throw std::runtime_error(message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno));
    }
    return result;
}


} // unix_
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F
