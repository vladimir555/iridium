#include "file_stream_reader.h"

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;


namespace iridium::io::fs::implementation {


CFileStreamReader::CFileStreamReader(std::string const &path)
:
    CFileStream(path, TOpenMode::READ)
{}


} // iridium::io::fs::implementation
