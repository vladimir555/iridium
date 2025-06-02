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
    m_shm_fd    (-1),
    m_shm_ptr   (nullptr),
    m_shm_size  (0),
    m_uri       (URI::create(uri)),
    m_is_opened (false)
{}


CSharedMemoryStreamPort::~CSharedMemoryStreamPort() {
    finalize();
}


void CSharedMemoryStreamPort::initialize() {
    if (m_is_opened)
        return;

    const std::string name = m_uri->getHost();
    m_shm_size = DEFAULT_BUFFER_SIZE;

    // Create shared memory object
    m_shm_fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    assertOK(m_shm_fd.load(), "shm_open failed");

    // Configure the size of the shared memory object
    assertOK(ftruncate(m_shm_fd, m_shm_size), "ftruncate failed");

    // Memory map the shared memory object
    m_shm_ptr = mmap(0, m_shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, m_shm_fd, 0);
    if (m_shm_ptr == MAP_FAILED) {
        throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
    }

    m_is_opened = true;
}


void CSharedMemoryStreamPort::finalize() {
    if (!m_is_opened)
        return;

    if (m_shm_ptr != nullptr) {
        munmap(m_shm_ptr, m_shm_size);
        m_shm_ptr = nullptr;
    }

    if (m_shm_fd != -1) {
        close(m_shm_fd);
        m_shm_fd = -1;
    }

    // Optional: unlink the shared memory object
    // shm_unlink(m_uri->getPath().c_str());

    m_is_opened = false;
}


size_t CSharedMemoryStreamPort::write(Buffer::TSharedPtr const &buffer) {
    if (!m_is_opened || m_shm_ptr == nullptr)
        throw std::runtime_error("shared memory port write error: not initialized");

    size_t size_to_write = buffer->size() > m_shm_size ? m_shm_size : buffer->size();
    memcpy(m_shm_ptr, buffer->data(), size_to_write);

    return size_to_write;
}


Buffer::TSharedPtr CSharedMemoryStreamPort::read(size_t const &size_) {
    if (!m_is_opened || m_shm_ptr == nullptr)
        throw std::runtime_error("shared memory port read error: not initialized");

    size_t size_to_read = (size_ == 0 || size_ > m_shm_size) ? m_shm_size : size_;
    auto buffer = Buffer::create(size_to_read);
    memcpy(buffer->data(), m_shm_ptr, size_to_read);

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
    // Shared memory operations are generally non-blocking by nature
    // This method is kept for interface compatibility
    m_is_blocking_mode = is_blocking;
}


} // unix_
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM
