#include "file_stream.h"

#include "iridium/items.h"

#include "file_api.h"
#include "file_api_assert.h"


using std::string;
using iridium::convertion::convert;


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
        close(m_file);
}


Buffer::TSharedPtr CFileStream::read(size_t const &size) {
    if (!m_file)
        throw std::runtime_error("file stream '" + m_file_name + "' not initialized"); // ----->

    auto buffer = Buffer::create(size, 0);
    auto count  = freadInternal(buffer->data(), 1, buffer->size(), m_file);

    if (count == 0)
        return nullptr; // ----->
    else
        buffer->resize(count);

    if (::ferror(m_file))
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


TFileStatus CFileStream::getStatus() const {
    try {
        return getFileStatus(m_file); // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error(
            "get status file '"  + m_file_name + "'" +
            " mode " + convert<string>(m_open_mode) +
            " error: " + e.what()); // ----->
    }
}


void CFileStream::initialize() {
    finalize();

    string open_mode;
    if (m_open_mode == TOpenMode::READ)
        open_mode = "rb";
    if (m_open_mode == TOpenMode::WRITE)
        open_mode = "abx";
    if (m_open_mode == TOpenMode::REWRITE)
        open_mode = "wbx";
//    if (m_open_mode.getEnums() == std::list<TOpenMode>{TOpenMode::READ, TOpenMode::WRITE})
//        open_mode = "rb+";

    try {
        if (open_mode.empty())
            throw std::runtime_error("empty open mode");
        m_file = open(m_file_name, open_mode);
    } catch (std::exception const &e) {
        throw std::runtime_error(
            "initialization file '" + m_file_name + "' mode " + convert<string>(m_open_mode) +
            " error: " + e.what()); // ----->
    }
}


void CFileStream::finalize() {
    try {
        if (m_file)
            close(m_file);
        m_file = nullptr;
    } catch (std::exception const &e) {
        throw std::runtime_error(
            "finalization file '" + m_file_name + "'" +
            " mode "   + convert<string>(m_open_mode) +
            " error: " + e.what()); // ----->
    }
}


std::list<uintptr_t> CFileStream::getHandles() const {
    if (m_file)
        return std::list<uintptr_t>{ static_cast<uintptr_t>(getFD(m_file)) }; // ----->
    else
        return {}; // ----->
}


URI::TSharedPtr CFileStream::getURI() const {
    return m_uri; // ----->
}


} // implementation
} // fs
} // io
} // iridium
