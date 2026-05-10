#ifndef HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
#define HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/items.h"
#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <cstring>
#include <string>
#include <atomic>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace iridium::io::implementation::platform {


/// \~english @brief Windows-specific stream port implementation.
///     This class is intended to provide a concrete implementation of `IStreamPort` for Windows,
///     likely using Windows-specific APIs like `HANDLE` for I/O operations.
///     It is non-copyable. The specifics of its behavior (e.g., for sockets, files, or pipes)
///     would be determined by its concrete implementation details in a corresponding .cpp file or derived classes.
/// \~russian @brief Реализация потокового порта для Windows.
///     Этот класс предназначен для предоставления конкретной реализации `IStreamPort` для Windows,
///     вероятно, используя специфичные для Windows API, такие как `HANDLE`, для операций ввода-вывода.
///     Класс является некопируемым. Особенности его поведения (например, для сокетов, файлов или каналов)
///     будут определяться деталями его конкретной реализации в соответствующем .cpp файле или производных классах.
class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
protected:
    CStreamPort(URI const &uri);
    virtual ~CStreamPort() = default;

public:
    virtual void
        initialize() override = 0;
    virtual void
        finalize() override = 0;
    size_t
        write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr
        read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    TMapHandleTypeIdent
        getHandles() const override;
    URI::TSharedPtr
        getURI() const override;

protected:
    DWORD
        assertOK(bool const &is_ok, std::string const &message);
    void
        setBlockingMode (bool const &is_blocking);
    void
        closeFDs();

    static int
        initSignal();

    HANDLE
        m_reader_fd;
    HANDLE
        m_writer_fd;
    HANDLE
        m_pid;

    OVERLAPPED
        m_reader_overlapped;
    OVERLAPPED
        m_writer_overlapped;
    OVERLAPPED
        m_pid_overlapped;

    URI::TSharedPtr
        m_uri;
    bool
        m_is_opened;
    bool
        m_is_blocking_mode;

    Buffer::TSharedPtr
        m_reader_buffer;
    Buffer::TSharedPtr
        m_writer_buffer;
};


} // iridium::io::implementation::platform


#endif  // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
