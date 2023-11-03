#ifndef HEADER_FILE_API_199F07F1_B426_44ED_AF7C_7FF69E5FDCC1
#define HEADER_FILE_API_199F07F1_B426_44ED_AF7C_7FF69E5FDCC1


#include "iridium/platform.h"


#if defined(LINUX_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)


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


#endif // LINUX_PLATFORM


#endif // HEADER_FILE_API_199F07F1_B426_44ED_AF7C_7FF69E5FDCC1
