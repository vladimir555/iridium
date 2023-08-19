#include "file_stream.h"

#include <cstring>
#include <sys/stat.h>

#include "iridium/items.h"
#include "iridium/platform.h"
#include "iridium/logging/logger.h"

#include "iridium/platform.h"

#ifdef WINDOWS_PLATFORM
#include <io.h>
#else
#include <sys/file.h>
#endif // WINDOWS_PLATFORM


using std::string;
using iridium::convertion::convert;


namespace {


// todo: refactor, move to platform
#include <iridium/macros/disable_warnings.h>
auto fopenInternal      = ::fopen;
auto fcloseInternal     = ::fclose;
auto fwriteInternal     = ::fwrite;
auto freadInternal      = ::fread;
auto fflushInternal     = ::fflush;
auto ferrorInternal     = ::ferror;
auto strerrorInternal   = ::strerror;
auto filenoInternal     = ::fileno;
#include <iridium/macros/enable_warnings.h>


void assertOK(const int &result, const string &message) {
    if (result != 0)
        throw std::runtime_error(message); // ----->
}


void assertOK(::FILE const * const result, const string &message) {
    if (result == nullptr)
        throw std::runtime_error(message); // ----->
}


} // unnamed


IMPLEMENT_ENUM(iridium::io::fs::implementation::CFileStream::TOpenMode)


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


CFileStream::CFileStream(string const &file_name, TOpenMode const &open_mode)
:
    m_file_name(file_name),
    m_open_mode(open_mode),
    m_uri      (URI::create("file://" + file_name))
{}


CFileStream::~CFileStream() {
    if (m_file)
        fcloseInternal(m_file);
}

    
Buffer::TSharedPtr CFileStream::read(size_t const &size) {
    if (!m_file)
        throw std::runtime_error("file stream '" + m_file_name + "' not initialized"); // ----->
    
    auto buffer = Buffer::create(size, 0);
    
    auto count = freadInternal(buffer->data(), 1, buffer->size(), m_file);
    
//    LOGT << "read size = " << count;
    if (count == 0) {
//        LOGT << "fd " << getID() << " empty read";
        return {}; // ----->
    } else {
        buffer->resize(count);
    }
    
    if (ferrorInternal(m_file))
        throw std::runtime_error(
            "read file '"       + m_file_name + "'" +
            " mode "            + convert<string>(m_open_mode) +
            " error: was read " + convert<string>(count) +
            " bytes, "          + strerrorInternal(errno)); // ----->
    
    return buffer; // ----->
}

    
size_t CFileStream::write(Buffer::TSharedPtr const &line) {
    if (!m_file)
        throw std::runtime_error("file stream '" + m_file_name + "' not initialized"); // ----->

    size_t count = fwriteInternal(line->data(), 1, line->size(), m_file);

    if (count != line->size())
        throw std::runtime_error(
            "write file '"          + m_file_name + "'" +
            " mode "                + convert<string>(m_open_mode) +
            " error: was written "  + convert<string>(count) +
            " from "                + convert<string>(line->size()) +
            " bytes: "              + strerrorInternal(errno)); // ----->

    return count; // ----->
}


void CFileStream::flush() {
    auto result = fflushInternal(m_file);
    assertOK(result,
        "flush file '"  + m_file_name + "'" +
        " mode "        + convert<string>(m_open_mode) +
        " error: "      + strerrorInternal(errno));
}


TFileStatus CFileStream::getStatus() {
    TFileStatus file_status = {};
    struct stat result      = {};

    assertOK(::fstat(getIDInternal(), &result),
         "get stat file '"  + m_file_name + "'" +
         " error: "         + strerrorInternal(errno));

#ifdef WINDOWS_PLATFORM
    auto tp = std::chrono::system_clock::from_time_t( result.st_mtime );
#endif
#ifdef FREEBSD_LIKE_PLATFORM
//#if defined(MACOS_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)
    std::chrono::system_clock::time_point tp {
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds       {result.st_mtimespec.tv_sec} +
            std::chrono::nanoseconds   {result.st_mtimespec.tv_nsec}
        )
    };
#endif
//#ifdef LINUX_PLATFORM
#if defined(LINUX_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)
    std::chrono::system_clock::time_point tp {
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds       {result.st_mtim.tv_sec} +
            std::chrono::nanoseconds   {result.st_mtim.tv_nsec}
        ) 
    };
#endif

    file_status.last_modified = tp;

    return file_status; // ----->
}


void CFileStream::initialize() {
    finalize();
    
    string open_mode;
    if (m_open_mode == TOpenMode::READ)
        open_mode = "rb";
    if (m_open_mode == TOpenMode::WRITE)
        open_mode = "ab";
    if (m_open_mode == TOpenMode::REWRITE)
        open_mode = "wb";
//    if (m_open_mode.getEnums() == std::list<TOpenMode>{TOpenMode::READ, TOpenMode::WRITE})
//        open_mode = "rb+";

    if (checkOneOf(m_open_mode, TOpenMode::WRITE, TOpenMode::REWRITE)) {
#ifdef WINDOWS_PLATFORM
        m_file = _fsopen(m_file_name.c_str(), open_mode.c_str(), _SH_DENYWR);
        assertOK(m_file,
            "initialization file '" + m_file_name + "'" +
            " mode " + convert<string>(m_open_mode) +
            " error: " + strerrorInternal(errno)); // ----->
#else
        m_file = fopenInternal(m_file_name.c_str(), open_mode.c_str());
        assertOK(m_file,
            "initialization file '" + m_file_name + "'" +
            " mode " + convert<string>(m_open_mode) +
            " error: " + strerrorInternal(errno)); // ----->
        assertOK(flock(getIDInternal(), LOCK_EX | LOCK_NB),
            "initialization file '" + m_file_name + "'" +
            " mode "   + convert<string>(m_open_mode) +
            " error: " + strerrorInternal(errno)); // ----->
#endif
    }
}


void CFileStream::finalize() {
    if (m_file) {
        if (checkOneOf(m_open_mode, TOpenMode::WRITE, TOpenMode::REWRITE)) {
#ifdef WINDOWS_PLATFORM
#else
            flock(getIDInternal(), LOCK_UN);
#endif
        }

        assertOK(fcloseInternal(m_file),
            "finalization file '" + m_file_name + "'" +
            " mode "   + convert<string>(m_open_mode) +
            " error: " + strerrorInternal(errno)); // ----->
        m_file = nullptr;
    }
}


int CFileStream::getIDInternal() const {
    // todo: move to separate headers
    if (m_file) {
#ifdef  LINUX_PLATFORM
        return fileno(m_file); // ----->
#endif
#ifdef  FREEBSD_LIKE_PLATFORM
        return m_file->_file; // ----->
#endif
#ifdef  WINDOWS_PLATFORM
        return filenoInternal(m_file); // ----->
#endif
    }
    throw std::runtime_error("file stream '" + m_file_name + "' get id error: not initialized"); // ----->
}


IStream::TID CFileStream::getID() const {
#ifdef  WINDOWS_PLATFORM
    return reinterpret_cast<TID>(_get_osfhandle(getIDInternal())); // ----->
#else
    return getIDInternal();
#endif
}


URI::TSharedPtr CFileStream::getURI() const {
    return m_uri;
}

    
    
size_t CFileStream::getSize() const {
#ifdef FREEBSD_LIKE_PLATFORM
    struct stat   file_stat = {};
#elif defined(WINDOWS_PLATFORM)
    struct stat   file_stat = {};
#else
    struct stat64 file_stat = {};
#endif

#ifdef FREEBSD_LIKE_PLATFORM
    auto result = fstat(filenoInternal(m_file), &file_stat);
#elif defined(WINDOWS_PLATFORM)
    auto result = fstat(filenoInternal(m_file), &file_stat);
#else
    auto result = fstat64(filenoInternal(m_file), &file_stat);
#endif
    assertOK(result,
        "get size file '" + m_file_name + "'" +
        " mode "   + convert<string>(m_open_mode) +
        " error: " + strerrorInternal(errno)); // ----->

    return static_cast<size_t>(file_stat.st_size); // ----->
}


} // implementation
} // fs
} // io
} // iridium
