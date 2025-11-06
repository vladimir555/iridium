#ifndef HEADER_FILE_API_0F3AC98A_8F86_4CA4_881B_2965E0C9560E
#define HEADER_FILE_API_0F3AC98A_8F86_4CA4_881B_2965E0C9560E


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include <cstdio>
#include <string>


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
namespace platform {


int getFD (::FILE *file);


namespace unix_ {


::FILE *open(const std::string &file_name, const std::string &open_mode);
void close(::FILE *file);


} // unix_
} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_FILE_API_0F3AC98A_8F86_4CA4_881B_2965E0C9560E
