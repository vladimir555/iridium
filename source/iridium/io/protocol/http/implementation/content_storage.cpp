#include "content_storage.h"

#include "iridium/io/fs/implementation/file_stream_reader.h"


using iridium::io::fs::implementation::CFileStreamReader;


namespace iridium {
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
} // iridium
