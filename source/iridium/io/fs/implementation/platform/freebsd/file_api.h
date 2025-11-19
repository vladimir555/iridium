#ifndef HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
#define HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/fs/file_status.h"
#include "../unix/file_api.h"

#include <string.h>


namespace iridium::io::fs::implementation::platform {


auto const fwriteInternal     = ::fwrite;
auto const freadInternal      = ::fread;
auto const fflushInternal     = ::fflush;
auto const strerrorInternal   = ::strerror;


int getFD(::FILE *file);


iridium::io::fs::TFileStatus getFileStatus(::FILE *file);


using unix_::open;
using unix_::close;


} // iridium::io::fs::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
