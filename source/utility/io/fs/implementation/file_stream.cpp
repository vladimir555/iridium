#include "file_stream.h"

#include <cstring>
#include <sys/stat.h>

#include <utility/convertion/convert.h>
#include <utility/platform.h>


using std::string;
using utility::convertion::convert;


namespace {


#include <utility/macros/disable_warnings.h>
auto fopenInternal      = ::fopen;
auto fcloseInternal     = ::fclose;
auto fwriteInternal     = ::fwrite;
auto freadInternal      = ::fread;
auto fflushInternal     = ::fflush;
auto ferrorInternal     = ::ferror;
auto strerrorInternal   = ::strerror;
#include <utility/macros/enable_warnings.h>


void assertOK(const int &result, const string &message) {
    if (result != 0)
        throw std::runtime_error(message); // ----->
}


void assertOK(::FILE const * const result, const string &message) {
    if (result == nullptr)
        throw std::runtime_error(message); // ----->
}


} // unnamed


IMPLEMENT_ENUM(utility::io::fs::implementation::CFileStream::TOpenMode)


namespace utility {
namespace io {
namespace fs {
namespace implementation {


CFileStream::CFileStream(string const &file_name, TOpenMode const &open_mode)
:
    m_file_name(file_name),
    m_open_mode(open_mode)
{}


CFileStream::~CFileStream() {
    if (m_file)
        fcloseInternal(m_file);
}

    
Buffer CFileStream::read(size_t const &size) {
    if (!m_file)
        throw std::runtime_error("file stream '" + m_file_name + "' not initialized"); // ----->
    
    Buffer buffer(size);
    
    auto count = freadInternal(buffer.data(), 1, buffer.size(), m_file);
    
    //    LOGT << "size = " << count << " buffer = '" << buffer << "'";
    if (count == 0)
        return {}; // ----->
    
    if (ferrorInternal(m_file))
        throw std::runtime_error(
            "read file '"       + m_file_name + "'" +
            " mode "            + convert<string>(m_open_mode) +
            " error: was read " + convert<string>(count) +
            " bytes, "          + strerrorInternal(errno)); // ----->
    
    return buffer; // ----->
}

    
size_t CFileStream::write(Buffer const &line) {
    if (!m_file)
        throw std::runtime_error("file stream '" + m_file_name + "' not initialized"); // ----->

    size_t count = fwriteInternal(line.data(), 1, line.size(), m_file);

    if (count != line.size())
        throw std::runtime_error(
            "write file '"          + m_file_name + "'" +
            " mode "                + convert<string>(m_open_mode) +
            " error: was written "  + convert<string>(count) +
            " from "                + convert<string>(line.size()) +
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


void CFileStream::initialize() {
    finalize();
    
    string open_mode;
    if (m_open_mode == TOpenMode::READ)
        open_mode = "rb";
    if (m_open_mode == TOpenMode::WRITE)
        open_mode = "ab+";
//    if (m_open_mode.getEnums() == std::list<TOpenMode>{TOpenMode::READ, TOpenMode::WRITE})
//        open_mode = "rb+";

    m_file = fopenInternal(m_file_name.c_str(), open_mode.c_str());
    assertOK(m_file,
         "initialize file '" + m_file_name + "'" +
         " mode "   + convert<string>(m_open_mode) +
         " error: " + strerrorInternal(errno)); // ----->
}


void CFileStream::finalize() {
    if (m_file) {
        auto result = fcloseInternal(m_file);
        assertOK(result,
            "finalize file '" + m_file_name + "'" +
            " mode "   + convert<string>(m_open_mode) +
            " error: " + strerrorInternal(errno)); // ----->
        m_file = nullptr;
    }
}


int CFileStream::getID() const {
    // todo: move to separate headers
    if (m_file)
#ifdef LINUX_PLATFORM
        return m_file->_fileno; // ----->
#elif  FREEBSD_LIKE_PLATFORM
        return m_file->_file; // ----->
#endif
    else
        return -1;
}
    
    
size_t CFileStream::getSize() const {
    struct stat file_stat = {};

    auto result = fstat(fileno(m_file), &file_stat);
    assertOK(result,
        "get size file '" + m_file_name + "'" +
        " mode "   + convert<string>(m_open_mode) +
        " error: " + strerrorInternal(errno)); // ----->

    return file_stat.st_size; // ----->
}


} // implementation
} // fs
} // io
} // utility
