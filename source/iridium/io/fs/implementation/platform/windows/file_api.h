#ifndef HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF
#define HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


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
  
std::string strerrorInternal(int const &code);

int getFD(::FILE *file);

iridium::io::fs::TFileStatus getFileStatus(::FILE *file);

::FILE *open(std::string const &file_name, std::string const &open_mode);

void close(::FILE *file);


} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF
