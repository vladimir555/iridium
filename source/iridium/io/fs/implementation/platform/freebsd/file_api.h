#ifndef HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
#define HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/fs/file_stream.h"

#include <string.h>


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
namespace platform {


auto const fwriteInternal     = ::fwrite;
auto const freadInternal      = ::fread;
auto const fflushInternal     = ::fflush;
auto const strerrorInternal   = ::strerror;
    
int getFD(::FILE *file);

iridium::io::fs::TFileStatus getFileStatus(::FILE *file);

::FILE *open(std::string const &file_name, std::string const &open_mode);

void close(::FILE *file);


} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
