#include "file_stream_writer.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
    
    
CFileStreamWriter::CFileStreamWriter(std::string const &path, bool const &is_rewrite)
:
    CFileStream(path, is_rewrite ? TOpenMode::REWRITE : TOpenMode::WRITE)
{}
    
    
} // implementation
} // fs
} // io
} // iridium
