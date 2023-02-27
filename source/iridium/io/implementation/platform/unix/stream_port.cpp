#include "stream_port.h"


#ifdef UNIX_PLATFORM


#include <unistd.h>
#include <fcntl.h>

#include "iridium/logging/logger.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


CStreamPort::CStreamPort(URL const &url)
:
    m_fd            (0),
    m_url           (URL::create(url)),
    m_is_opened     (false)
{}


size_t CStreamPort::write(Buffer::TSharedPtr const &buffer_) {
    if (!m_fd)
        throw std::runtime_error("stream port read error: not initialized"); // ----->

    auto buffer = static_cast<void const *>(buffer_->data());
    auto size   = DEFAULT_BUFFER_SIZE;

    if (size > buffer_->size())
        size = buffer_->size();

    auto result = ::write(m_fd, buffer, size);

//    LOGT << __FUNCTION__ << " fd: " << m_fd << ", size: " << size << ", buffer:\n"
//         << static_cast<char const *>(buffer);

    if (result < 0) {
        if (errno == EAGAIN)
            return 0;
        else
            assertOK(result, "write error");
    }

    // sent async
    if (result == EAGAIN ||
        result == EWOULDBLOCK
    )
        return
        buffer_->size() < DEFAULT_BUFFER_SIZE ?
        buffer_->size() : DEFAULT_BUFFER_SIZE; // ----->
    else
        return result; // ----->

    return buffer_->size(); // ----->
}


Buffer::TSharedPtr CStreamPort::read(size_t const &size_) {
    if (!m_fd)
        throw std::runtime_error("stream port read error: not initialized"); // ----->

    auto const size = size_ == 0 ? DEFAULT_BUFFER_SIZE : size_;
    char buffer[size];
    auto result = ::read(m_fd, buffer, size - 1);

    if (result < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return Buffer::create();
        else
            assertOK(result, "write error");
    }

//    LOGT << __FUNCTION__ << " fd: " << m_fd << ", result: " << result << ", buffer:\n---\n"
//         << std::string(buffer, buffer + result) << "\n---\nerrno: " << std::strerror(errno);

//    if(received_size == EOF || !m_is_blocking_mode) {
//        if (received_size == -1 && errno == EAGAIN)
//            return Buffer::create(); // ----->
//        else
//            assertOK(received_size, "read error");
//    } else
//        received_size = 0;

    return Buffer::create(Buffer(buffer, buffer + result)); // ----->
}


int CStreamPort::getID() const {
    if (m_fd)
        return m_fd; // ----->

    throw std::runtime_error("stream port get id error: not initialized"); // ----->
}


void CStreamPort::setBlockingMode(int const &socket, bool const &is_blocking) {
    auto flags = assertOK(fcntl(socket, F_GETFL, 0), "get flag error");
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(socket, F_SETFL, flags), "set flag error");
    m_is_blocking_mode = is_blocking;
}


} // unix_
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM
