#include "file_stream_writer.h"


namespace iridium::io::fs::implementation {


CFileStreamWriter::CFileStreamWriter(std::string const &path, bool const &is_rewrite)
:
    CFileStream(path, is_rewrite ? TOpenMode::REWRITE : TOpenMode::WRITE)
{}


} // iridium::io::fs::implementation
