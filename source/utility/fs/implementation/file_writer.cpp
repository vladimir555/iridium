#include "file_writer.h"

#include <cstring>

#include <utility/convertion/convert.h>


using std::string;
using utility::convertion::convert;


namespace {


#include <utility/macros/disable_warnings.h>
auto fopenInternal      = ::fopen;
auto fcloseInternal     = ::fclose;
auto fwriteInternal     = ::fwrite;
auto fflushInternal     = ::fflush;
auto strerrorInternal   = ::strerror;
#include <utility/macros/enable_warnings.h>


void assertOK(const int &result, const string &message) {
    if (result != 0)
        throw std::runtime_error(message);
}


void assertOK(::FILE const * const result, const string &message) {
    if (result == nullptr)
        throw std::runtime_error(message);
}


} // unnamed


namespace utility {
namespace fs {
namespace implementation {


CFileWriter::CFileWriter(string const &file_name)
:
    m_file_name(file_name) 
{}


CFileWriter::~CFileWriter() {
    if (m_file)
        fcloseInternal(m_file);
}


size_t CFileWriter::write(io::TBuffer const &line) {
    if (!m_file)
        throw std::runtime_error("fast text writer not initialized"); // ----->

    size_t	count = fwriteInternal(line.data(), 1, line.size(), m_file);

    if (count != line.size())
        throw std::runtime_error(
        "write to query cache file '" + m_file_name +
        "' error: was written " + convert<string>(count) +
        " from " + convert<string>(line.size()) + " bytes: " + strerrorInternal(errno)); // ----->

    return count; // ----->
}


void CFileWriter::flush() {
    auto result = fflushInternal(m_file);
    assertOK(result, "write to file '" + m_file_name + "' error, flush error: " + strerrorInternal(errno));
}


void CFileWriter::initialize() {
    finalize();
    m_file = fopenInternal(m_file_name.c_str(), "ab");
    assertOK(m_file, "initialize file " + m_file_name + " error: " + strerrorInternal(errno)); // ----->
}


void CFileWriter::finalize() {
    if (m_file) {
        auto result = fcloseInternal(m_file);
        assertOK(result, "finalize file " + m_file_name + " error: " + strerrorInternal(errno)); // ----->
        m_file = nullptr;
    }
}


//int CFileWriter::getID() const override {
//    if (m_file)
//        return m_file->_file; // ----->
//    else
//        return -1;
//}


} // implementation
} // fs
} // utility