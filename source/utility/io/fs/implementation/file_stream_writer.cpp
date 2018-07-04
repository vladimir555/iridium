#include "file_stream_writer.h"

#include "utility/convertion/convert.h"


using utility::convertion::convert;


namespace utility {
namespace io {
namespace fs {
namespace implementation {
    
    
CFileStreamWriter::CFileStreamWriter(std::string const &path)
:
    CFileStream(path, TOpenMode::WRITE)
{}


void CFileStreamWriter::initialize() {
    CFileStream::initialize();
};


void CFileStreamWriter::finalize() {
    CFileStream::finalize();
};


int CFileStreamWriter::getID() const {
    return CFileStream::getID(); // ----->
}


size_t CFileStreamWriter::write(Buffer const &buffer) {
    return CFileStream::write(buffer); // ----->
}


void CFileStreamWriter::flush() {
    return CFileStream::flush(); // ----->
}
    
    
} // implementation
} // fs
} // io
} // utility
