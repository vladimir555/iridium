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


class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
protected:
    CStreamPort(URI const &uri);
    virtual ~CStreamPort() = default;

public:
    virtual void            initialize() override = 0;
    virtual void            finalize() override = 0;
    size_t                  write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr      read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    std::list<uintptr_t>    getHandles() const override;
    URI::TSharedPtr         getURI() const override;

protected:
    DWORD   checkResult     (bool const &is_ok, std::string const &message);
    void    setBlockingMode (bool const &is_blocking);
    void    closeFDs();

    static int initSignal();

    HANDLE              m_reader_fd;
    HANDLE              m_writer_fd;

    OVERLAPPED          m_reader_overlapped;
    OVERLAPPED          m_writer_overlapped;

    URI::TSharedPtr     m_uri;
    bool                m_is_opened;
    bool                m_is_blocking_mode;

    Buffer::TSharedPtr m_reader_buffer;
    Buffer::TSharedPtr m_writer_buffer;
};


} // iridium::io::implementation::platform


#endif  // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
