#include "file_stream_writer.h"

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
    
    
CFileStreamWriter::CFileStreamWriter(std::string const &path, bool const &is_rewrite)
:
    CFileStream(path, is_rewrite ? TOpenMode::REWRITE : TOpenMode::WRITE)
{}


void CFileStreamWriter::initialize() {
    CFileStream::initialize();
};


void CFileStreamWriter::finalize() {
    CFileStream::finalize();
};


IStream::TID CFileStreamWriter::getID() const {
    return CFileStream::getID(); // ----->
}


URI::TSharedPtr CFileStreamWriter::getURI() const {
    return CFileStream::getURI();
}


size_t CFileStreamWriter::write(Buffer::TSharedPtr const &buffer) {
    return CFileStream::write(buffer); // ----->
}


void CFileStreamWriter::flush() {
    return CFileStream::flush(); // ----->
}
    
    
} // implementation
} // fs
} // io
} // iridium
