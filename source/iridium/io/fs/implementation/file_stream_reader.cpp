#include "file_stream_reader.h"

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace fs {
namespace implementation {

    
CFileStreamReader::CFileStreamReader(std::string const &path)
:
    CFileStream(path, TOpenMode::READ)
{}


} // implementation
} // fs
} // io
} // iridium
