#include "content_storage.h"

#include "iridium/io/fs/implementation/file_stream_reader.h"

#include "iridium/logging/logger.h"
#include "iridium/io/fs/files.h"

#include <cctype>


using iridium::io::fs::implementation::CFileStreamReader;
using std::string;


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


CContentStorage::CContentStorage(string const &root_path)
:
    m_root_path(root_path)
{}


fs::IFileStreamReader::TSharedPtr CContentStorage::getContent(string const &path) {
//    LOGT << "root_path = " << m_root_path << " path = " << path;

    bool is_path_checked = true;
    unsigned char prev_ch = '\x00';
    for (auto const &ch: path) {
        if (!(std::isalpha(ch) || std::isdigit(ch) || ch == '/' || ch == '.') || (ch == '.' && prev_ch == '.')) {
            is_path_checked = false;
            break; // --->
        }
        prev_ch = ch;
    }

    if (is_path_checked && fs::checkFileExistence(m_root_path + path))
        return CFileStreamReader::create(m_root_path + path); // ----->

    return nullptr; // ----->
}


} // implementation
} // http
} // protocol
} // io
} // iridium
