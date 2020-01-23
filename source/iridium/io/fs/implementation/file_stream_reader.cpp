/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
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

    
void CFileStreamReader::initialize() {
    CFileStream::initialize();
};


void CFileStreamReader::finalize() {
    CFileStream::finalize();
};


int CFileStreamReader::getID() const {
    return CFileStream::getID(); // ----->
}


Buffer::TSharedPtr CFileStreamReader::read(size_t const &size) {
    return CFileStream::read(size); // ----->
}

    
size_t CFileStreamReader::getSize() {
    return CFileStream::getSize(); // ----->
}


TFileStatus CFileStreamReader::getStatus() {
    return CFileStream::getStatus(); // ----->
}


} // implementation
} // fs
} // io
} // iridium
