#include "file_stream_reader.h"

#include "utility/convertion/convert.h"


using utility::convertion::convert;


namespace utility {
namespace io {
namespace fs {
namespace implementation {

    
CFileStreamReader::CFileStreamReader(std::string const &path)
:
    CFileStream(path, TOpenMode::READ)
{}

    
void CFileStreamReader::initialize() {
    CFileStream::initialize();
};


void CFileStreamReader::finalize() {
    CFileStream::finalize();
};


int CFileStreamReader::getID() const {
    return CFileStream::getID(); // ----->
}


Buffer CFileStreamReader::read(size_t const &size) {
    return CFileStream::read(size); // ----->
}

    
size_t CFileStreamReader::getSize() {
    return CFileStream::getSize(); // ----->
}


} // implementation
} // fs
} // io
} // utility
