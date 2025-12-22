#include "stream_port.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/assert.h"
#include "iridium/strings.h"
#include "iridium/convertion/convert.h"
#include "iridium/io/event.h"

#include <list>


using iridium::assertExists;
using iridium::convertion::convert;
using std::string;


#include "iridium/logging/logger.h"
namespace iridium::io::implementation::platform {


CStreamPort::CStreamPort(URI const &uri)
:
    m_reader_fd         (0),
    m_writer_fd         (0),
    m_writer_overlapped {0},
    m_reader_overlapped {0},
    m_uri               (URI::create(uri)),
    m_is_opened         (false)
{}


std::list<uintptr_t> CStreamPort::getHandles() const {
    std::list<uintptr_t> handles;

    // if (m_writer_fd && m_writer_fd != INVALID_HANDLE_VALUE)
        handles.push_back(reinterpret_cast<uintptr_t>(m_writer_fd));

    // if (m_reader_fd && m_reader_fd != INVALID_HANDLE_VALUE && m_reader_fd != m_writer_fd)
        handles.push_back(reinterpret_cast<uintptr_t>(m_reader_fd));

    return handles; // ----->
}


URI::TSharedPtr CStreamPort::getURI() const {
    return m_uri;
}


DWORD CStreamPort::checkResult(bool const &is_ok, std::string const &message) {
    using convertion::convert;
    using std::string;

    // LOGT << "check: " << message;

    if (is_ok) {
        return ERROR_SUCCESS;
    } else {
        auto error_code = ::GetLastError();

        if (checkOneOf<int32_t>(
            error_code,
            ERROR_IO_INCOMPLETE,
            ERROR_BROKEN_PIPE,
            ERROR_HANDLE_EOF,
            ERROR_IO_PENDING)
        )
            return error_code; // ----->

        LPSTR  buffer = nullptr;
        size_t size   = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER  |
            FORMAT_MESSAGE_FROM_SYSTEM      |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_code,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&buffer), 0, NULL);

        std::string api_message(buffer, size);
        LocalFree(buffer);

        throw std::runtime_error(
            message + ": " + trim(api_message) + " ("+
            convert<string, uint64_t>(error_code, uint8_t{16}) + "), uri '" +
            convert<string>(m_uri) + "'");
            // ----->
    }
}


void CStreamPort::setBlockingMode(bool const &is_blocking) {
    m_is_blocking_mode = is_blocking;
}


void CStreamPort::closeFDs() {
    if (!m_writer_fd && !m_reader_fd)
        throw std::runtime_error("closing error: not initialized");

    if (m_reader_fd) {
        CloseHandle(m_reader_fd);
        m_reader_fd = nullptr;
        m_reader_overlapped = { 0 };
    }

    if (m_writer_fd) {
        CloseHandle(m_writer_fd);
        m_writer_fd = nullptr;
        m_writer_overlapped = { 0 };
    }
}


Buffer::TSharedPtr CStreamPort::read(size_t const &size) {
    try {
        if (!m_reader_fd || m_reader_fd == INVALID_HANDLE_VALUE)
            throw std::runtime_error("reading error: not initialized");

        auto  buffer = Buffer::create();
        DWORD bytes_read = 0;

        auto result = checkResult(
            GetOverlappedResult(m_reader_fd, &m_reader_overlapped, &bytes_read, FALSE),
           "GetOverlappedResult");

        if  (result == ERROR_SUCCESS && m_reader_buffer) {
            m_reader_buffer->resize(bytes_read);
            buffer = m_reader_buffer;
        } else {
            if (m_reader_buffer)
                return buffer;
        }

        m_reader_buffer     = Buffer::create(size, uint8_t{0});
        m_reader_overlapped = { 0 };
        m_reader_overlapped.Pointer = reinterpret_cast<PVOID>(Event::TOperation::READ);

        checkResult(
            ReadFile(
                m_reader_fd,
                m_reader_buffer->data(),
                static_cast<DWORD>(m_reader_buffer->size()),
                nullptr,
                &m_reader_overlapped),
            "ReadFile");

        return buffer; // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error(convert<string>(m_uri) + "reading error: " + e.what());
    }
}


size_t CStreamPort::write(Buffer::TSharedPtr const &buffer) {
    try {
        if (!m_writer_fd || m_writer_fd == INVALID_HANDLE_VALUE)
            throw std::runtime_error("writing error: not initialized");

        if (!buffer || buffer->empty())
            return 0;

        DWORD bytes_written = 0;

        auto result = checkResult(
            GetOverlappedResult(
                m_writer_fd,
                &m_writer_overlapped,
                &bytes_written,
                FALSE
            ),
            "GetOverlappedResult"
        );

        if (result == ERROR_SUCCESS && m_writer_buffer) {
            m_writer_buffer.reset();
        } else {
            if (m_writer_buffer)
                return 0;
        }

        m_writer_buffer     = buffer;
        m_writer_overlapped = {};
        m_writer_overlapped.Pointer = reinterpret_cast<PVOID>(Event::TOperation::WRITE);

        checkResult(
            WriteFile(
                m_writer_fd,
                buffer->data(),
                static_cast<DWORD>(buffer->size()),
                nullptr,
                &m_writer_overlapped
            ),
            "WriteFile"
        );

        return bytes_written; // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error(
            convert<std::string>(m_uri) + "writing error: " + e.what()
        );
    }
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
