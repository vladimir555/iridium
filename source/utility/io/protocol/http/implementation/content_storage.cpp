#include "content_storage.h"

#include "utility/io/fs/implementation/file_stream_reader.h"


using utility::io::fs::implementation::CFileStreamReader;


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


CContentStorage::CContentStorage(std::string const &root_path)
:
    m_root_path(root_path)
{}


fs::IFileStreamReader::TSharedPtr CContentStorage::getContent(std::string const &path) {
    auto   stream_reader = CFileStreamReader::create(m_root_path + "/" + path);
    stream_reader->initialize();
    // todo: fix finalize
    return stream_reader; // ----->
};


} // implementation
} // http
} // protocol
} // io
} // utility
