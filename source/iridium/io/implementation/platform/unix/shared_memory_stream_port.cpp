#include "shared_memory_stream_port.h"


#ifdef UNIX_PLATFORM


#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


CSharedMemoryStreamPort::CSharedMemoryStreamPort(URI const &uri)
:
    m_shm_fd            (-1),
    m_shm_ptr           (nullptr),
    m_data_ptr          (nullptr),
    m_shm_size          (DEFAULT_BUFFER_SIZE),
    m_uri               (URI::create(uri)),
    m_is_opened         (false),
    m_is_blocking_mode  (false),
    m_refcount          (nullptr)
{}


CSharedMemoryStreamPort::~CSharedMemoryStreamPort() {
    finalize();
}


void CSharedMemoryStreamPort::initialize() {
    if (m_is_opened)
        return;

    m_shm_name = "/" + m_uri->getPath();
    if (m_shm_name.empty() || m_shm_name[0] != '/') {
        throw std::runtime_error("shared memory name must start with '/': " + m_shm_name);
    }

    size_t total_size = m_shm_size + sizeof(std::atomic<int>);
    m_shm_fd = shm_open(m_shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    assertOK(m_shm_fd.load(), "shm_open failed");

    assertOK(ftruncate(m_shm_fd, total_size), "ftruncate failed");

    m_shm_ptr = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_shm_fd, 0);
    if (m_shm_ptr == MAP_FAILED) {
        throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
    }

    m_refcount = new (m_shm_ptr) std::atomic<int>(1);
    m_data_ptr = static_cast<char*>(m_shm_ptr) + sizeof(std::atomic<int>);
    m_is_opened = true;
}


void CSharedMemoryStreamPort::finalize() {
    if (!m_is_opened)
        return;

    if (m_shm_ptr) {
        if (m_refcount && --(*m_refcount) == 0) {
            shm_unlink(m_shm_name.c_str());
        }

        size_t total_size = m_shm_size + sizeof(std::atomic<int>);
        munmap(m_shm_ptr, total_size);
        m_shm_ptr = nullptr;
        m_data_ptr = nullptr;
        m_refcount = nullptr;
    }

    if (m_shm_fd != -1) {
        close(m_shm_fd);
        m_shm_fd = -1;
    }

    m_is_opened = false;
}


size_t CSharedMemoryStreamPort::write(Buffer::TSharedPtr const &buffer) {
    if (!m_is_opened || !m_data_ptr)
        throw std::runtime_error("shared memory port write error: not initialized");

    size_t size_to_write = std::min(buffer->size(), m_shm_size);
    memcpy(m_data_ptr, buffer->data(), size_to_write);

    return size_to_write;
}


Buffer::TSharedPtr CSharedMemoryStreamPort::read(size_t const &size_) {
    if (!m_is_opened || !m_data_ptr)
        throw std::runtime_error("shared memory port read error: not initialized");

    size_t size_to_read = (size_ == 0 || size_ > m_shm_size) ? m_shm_size : size_;
    auto buffer = Buffer::create(size_to_read);
    memcpy(buffer->data(), m_data_ptr, size_to_read);

    return buffer;
}


std::list<uintptr_t> CSharedMemoryStreamPort::getHandles() const {
    std::list<uintptr_t> handles;

    if (m_shm_fd != -1)
        handles.push_back(m_shm_fd);

    return handles;
}


URI::TSharedPtr CSharedMemoryStreamPort::getURI() const {
    return m_uri;
}


void CSharedMemoryStreamPort::setBlockingMode(bool const &is_blocking) {
    m_is_blocking_mode = is_blocking;
}


} // unix_
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM
